//
// Created by roman on 5/1/19.
//

#include "keychain_commands.hpp"
#include "keyfile_singleton.hpp"
#include "keychain_logger.hpp"

#include <sqlite3/sqlite3.h>


using namespace keychain_app;

int database_connection::callback(void *checker_value_, int columns_num, char **row, char **column_name)
{
  //NOTE: I don't want to throw exception into callback function, because of possible errors in sqlite library.
  auto& log = logger_singleton::instance();
  assert(columns_num == 1);
  if(columns_num != 1)
    BOOST_LOG_SEV(log.lg, info) << "database_connection::callback(): invalid num of columns = " << columns_num;
  assert(column_name[0]);
  if(!column_name[0])
    BOOST_LOG_SEV(log.lg, info) << "database_connection::callback(): column name is NULL ";
  uint8_t* checker_value = (uint8_t*)checker_value_;
  if(!strcmp(column_name[0], "keyfiles_table_name"))
  {
    if(row[0] && !strcmp(row[0], "keyfiles"))
      *checker_value |= CHECK_KEYFILES_TABLE;
  }
  if(!strcmp(column_name[0], "signlogs_table_name"))
  {
    if(row[0] && !strcmp(row[0], "sign_logs"))
      *checker_value |= CHECK_SIGNLOGS_TABLE;
  }
  return 0;
}

database_connection::database_connection(const char* db_name)
{
  char *err_msg = 0;
  try {
    auto rc = sqlite3_open(db_name, &m_db);
    if (rc)
      FC_LIGHT_THROW_EXCEPTION(fc_light::keyfiles_db_cannot_open_exception, "sqlite open error: %{ERRMSG}",
                               ("ERRMSG", sqlite3_errmsg(m_db)));
    table_check_result check_tables_result = CHECK_NULL;
    rc = sqlite3_exec(m_db, check_keyfiles_tables, callback, &check_tables_result, &err_msg);
    if( rc!=SQLITE_OK )
      FC_LIGHT_THROW_EXCEPTION(fc_light::keyfiles_db_cannot_open_exception, "keyfiles check initialize error: %{ERRMSG}", ("ERRMSG", err_msg));\
    switch (check_tables_result)
    {
      case CHECK_NULL: rc = sqlite3_exec(m_db, create_keyfiles_tables, nullptr, &check_tables_result, &err_msg);//TODO: need to figure out is it need to check result rows?
        break;
      case CHECK_KEYFILES_TABLE:
      case CHECK_SIGNLOGS_TABLE:
        FC_LIGHT_ASSERT(false);
      case CHECK_FULL:
        break;
      default:
        break;
    }
  }
  catch (fc_light::exception& exc)
  {
    sqlite3_close(m_db);
    if(err_msg)
      sqlite3_free(err_msg);
    FC_LIGHT_RETHROW_EXCEPTION(exc, error, "");
  }
}

database_connection::~database_connection()
{
  sqlite3_close(m_db);
}

keyfile_singleton& keyfile_singleton::instance()
{
  static keyfile_singleton keyfile_map;
  return keyfile_map;
}

keyfile_singleton::const_iterator keyfile_singleton::begin() const
{
  return m_keydata_map.begin();
}

keyfile_singleton::const_iterator keyfile_singleton::end() const
{
  return m_keydata_map.end();
}

const keyfile_singleton::prim_index_type& keyfile_singleton::prim_index() const
{
  return m_keydata_map.get<keyfiles_map::prim_pubkey_tag>();
}

const keyfile_singleton::second_index_type& keyfile_singleton::second_index() const
{
  return m_keydata_map.get<keyfiles_map::second_keyname_tag>();
}

const keyfile_singleton::third_index_type& keyfile_singleton::third_index() const
{
  return m_keydata_map.get<keyfiles_map::third_date_tag>();
}

keyfile_singleton::keyfile_singleton()
{
  auto print_exception = [](const auto& filename, fc_light::exception &er) {
    auto& log = logger_singleton::instance();
    BOOST_LOG_SEV(log.lg, warning) << "Cannot read key file \"" << filename.c_str() << "\"" << er.to_detail_string();
  };
  try
  {
    auto curdir = bfs::current_path();
    auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
    std::for_each(first, bfs::directory_iterator(), [&print_exception, this](const auto& unit) {
      try {
        fc_light::variant j_keyfile = open_keyfile(unit.path().c_str());
        m_keydata_map.insert(j_keyfile.as<keyfile_format::keyfile_t>());
      }
      catch (fc_light::parse_error_exception& er) {
        return print_exception(unit.path(), er);
      }
      catch (fc_light::bad_cast_exception& er) {
        return print_exception(unit.path(), er);
      }
      FC_LIGHT_RETHROW_EXCEPTIONS(log_level::error, "Cannot parse key file \"${KEYFILE}\"", ("KEYFILE", unit.path().c_str()))
    });
  }
  FC_LIGHT_RETHROW_EXCEPTIONS(log_level::error, "Cannot create keyfiles map")
}

keyfile_singleton::~keyfile_singleton()
{
  flush_all();
}

const keyfile_format::keyfile_t& keyfile_singleton::operator[](const keyfile_singleton::prim_key_type& key) const
{
  auto it = m_keydata_map.find(key);
  if (it == m_keydata_map.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist public = ${key_}", ("key_", key));
  return *it;
}

const keyfile_format::keyfile_t& keyfile_singleton::operator[](const keyfile_singleton::second_key_type& key) const
{
  auto& second_index = m_keydata_map.get<1>();
  auto it = second_index.find(key);
  if (it == second_index.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not found by keyname = ${name_}", ("name_", key));
  return *it;
}

void keyfile_singleton::update(keyfile_format::keyfile_t&& keyfile)
{
  auto it = m_keydata_map.find(keyfile.keyinfo.public_key);
  if (it == m_keydata_map.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist, public = ${key_}", ("key_", keyfile.keyinfo.public_key));
  auto res = m_keydata_map.replace(it, keyfile);
  if (!res)
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot replace keyfile in map, public = ${key_}", ("key_", keyfile.keyinfo.public_key));
}

void keyfile_singleton::insert(keyfile_format::keyfile_t&& keyfile_data)
{
  auto res = m_keydata_map.insert(keyfile_data);
  if (res.second == false)
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Keyfile with name \"${key_}\" is already exist", ("key_", keyfile_data.keyinfo.public_key));
  flush_keyfile(keyfile_data.keyinfo.public_key);
}

bool keyfile_singleton::is_exist(const keyfile_singleton::prim_key_type& key) const
{
  auto it = m_keydata_map.find(key);
  return it != m_keydata_map.end();
}

void keyfile_singleton::flush_keyfile(const keyfile_singleton::prim_key_type& key) const
{
  auto it = m_keydata_map.find(key);
  if (it == m_keydata_map.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist public = ${key_}", ("key_", key));
  flush_keyfile_impl(*it);
}

void keyfile_singleton::flush_keyfile_impl(const value_t& keyfile_data) const
{
  auto hash = dev::openssl::sha3(keyfile_data.keyinfo.public_key);
  auto filename = hash.hex().substr(0, 16);
  filename += ".json";
  bfs::path filepath(std::string(KEY_DEFAULT_PATH_"/") + std::string(filename));
  auto fout = std::ofstream(filepath.c_str());
  if (!fout.is_open())
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile (${filename})", ("filename", filename));
  fout << fc_light::json::to_pretty_string(keyfile_data) << std::endl;
}

void keyfile_singleton::flush_all() const
{
  auto curdir = bfs::current_path();
  auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
  std::for_each(m_keydata_map.begin(), m_keydata_map.end(), [first](const auto& keyfile_data) {
    auto hash = dev::openssl::sha3(keyfile_data.keyinfo.public_key);
    auto filename = hash.hex().substr(0, 16);
    filename += ".json";
    bfs::path filepath(std::string(KEY_DEFAULT_PATH_"/") + std::string(filename));
    auto fout = std::ofstream(filepath.c_str());
    if (!fout.is_open())
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile (${filename})", ("filename", filename));
    fout << fc_light::json::to_pretty_string(keyfile_data) << std::endl;
  });
}