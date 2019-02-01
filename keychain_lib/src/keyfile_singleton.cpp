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