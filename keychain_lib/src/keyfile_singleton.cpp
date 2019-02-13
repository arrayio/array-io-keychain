//
// Created by roman on 5/1/19.
//

#include "keychain_commands.hpp"
#include "keyfile_singleton.hpp"

using namespace keychain_app;

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

const keyfile_singleton::random_access_index_type& keyfile_singleton::random_access_index() const
{
  return m_keydata_map.get<keyfiles_map::random_access_tag>();
}

void keyfile_singleton::keydata_initialize()
{
  auto curdir = bfs::current_path();
  auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
  std::for_each(first, bfs::directory_iterator(), [this](const auto& unit) {
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

void keyfile_singleton::signlog_initialize()
{
  auto curdir = bfs::current_path();
  auto first = bfs::directory_iterator(bfs::path(SIGN_LOGS_DEFAULT_PATH_));
  std::for_each(first, bfs::directory_iterator(), [this](const auto& unit) {
    try {
      fc_light::variant j_keyfile = open_keyfile(unit.path().c_str());
      auto file = j_keyfile.as<keyfile_format::signlog_file_t>();
      auto res = m_signlog_map.insert(signlog_map_t::value_type(file.public_key,log_records_t()));
      FC_LIGHT_ASSERT(res.second);
      auto it = res.first;
      auto& logmap = it->second;
      std::copy(file.sign_events.begin(), file.sign_events.end(), std::inserter(logmap, logmap.begin()));
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

void keyfile_singleton::print_exception (const boost::filesystem::path& filename, fc_light::exception &er)
{
  auto& log = logger_singleton::instance();
  BOOST_LOG_SEV(log.lg, warning) << "Cannot read key file \"" << filename.c_str() << "\"" << er.to_detail_string();
};

keyfile_singleton::keyfile_singleton()
{
  
  try
  {
    keydata_initialize();
    signlog_initialize();
  }
  FC_LIGHT_RETHROW_EXCEPTIONS(log_level::error, "Cannot create keyfiles map")
}

keyfile_singleton::~keyfile_singleton()
{
  flush_all();
}

const keyfile_format::keyfile_t& keyfile_singleton::operator[](const keyfile_singleton::prim_key_type& key)
{
  bool stop = false;
  do{
    auto it = m_keydata_map.find(key);
    if (it == m_keydata_map.end())
    {
      keydata_initialize();
      if(stop == true)
        FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not exist public = ${key_}", ("key_", key));
      stop = true;
    }
    return *it;
  } while (true);
  
}

const keyfile_format::keyfile_t& keyfile_singleton::operator[](const keyfile_singleton::second_key_type& key)
{
  bool stop = false;
  do {
    auto &second_index = m_keydata_map.get<1>();
    auto it = second_index.find(key);
    if (it == second_index.end())
    {
      keydata_initialize();
      if(stop == true)
        FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Private key is not found by keyname = ${name_}",
                               ("name_", key));
      stop = true;
    }
    return *it;
  } while(true);
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

void keyfile_singleton::flush_logrecords(const prim_key_type& key) const
{
  auto it = m_signlog_map.find(key);
  if (it == m_signlog_map.end())
    return;
  flush_logrecords_impl(key, it->second);
}

void keyfile_singleton::flush_logrecords_impl(const prim_key_type& key, const log_records_t& log_records) const
{
  keyfile_format::signlog_file_t logfile;
  logfile.public_key = key;
  std::copy(log_records.begin(), log_records.end(), std::back_inserter(logfile.sign_events));
  auto hash = dev::openssl::sha3(key);
  auto filename = hash.hex().substr(0, 16);
  filename += "_signlog.json";
  bfs::path filepath(std::string(SIGN_LOGS_DEFAULT_PATH_"/") + std::string(filename));
  auto fout = std::ofstream(filepath.c_str());
  if (!fout.is_open())
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile (${filename})", ("filename", filename));
  fout << fc_light::json::to_pretty_string(logfile) << std::endl;
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
  std::for_each(m_signlog_map.begin(), m_signlog_map.end(), [this](const auto& val){
    flush_logrecords(val.first);
  });
}

const keyfile_singleton::log_index_type& keyfile_singleton::get_logs(const dev::Public& pkey)
{
  signlog_initialize();//NOTE: it may be slowly, using sqlite and triggers is more preferable
  auto it = m_signlog_map.find(pkey);
  if (it == m_signlog_map.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::file_not_found_exception, "Public_key: ${PKEY}", ("PKEY", pkey));
  auto& records = it->second;
  return records.get<keyfiles_map::log_record_tag>();
}

void keyfile_singleton::add_log_record(const dev::Public& pkey, const keyfile_format::log_record& record)
{
  auto it = m_signlog_map.find(pkey);
  if (it == m_signlog_map.end())
  {
    it = m_signlog_map.insert(m_signlog_map.begin(), std::pair<dev::Public, log_records_t>(pkey,log_records_t()));
    if (it == m_signlog_map.end())
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot insert log record, public_key: ${PKEY}", ("PKEY", pkey));
  }
  it->second.insert(record);
  flush_logrecords(pkey);
}