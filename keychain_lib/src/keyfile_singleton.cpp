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

keyfile_singleton::keyfile_singleton()
{
  try
  {
    auto curdir = bfs::current_path();
    auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
    std::for_each(first, bfs::directory_iterator(), [this](const auto& unit) {
      try {
        fc_light::variant j_keyfile = open_keyfile(unit.path().c_str());
        auto file_data = j_keyfile.as<keyfile_format::keyfile_t>();
        m_keydata_map.insert(value_t(file_data.keyname, file_data));
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

keyfile_format::keyfile_t& keyfile_singleton::operator[](const std::string& key)
{
  auto it = m_keydata_map.find(key);
  if( it == m_keydata_map.end() )
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Keyname is not exist (${key_})", ("key_", key));
  return it->second;
}

void keyfile_singleton::insert(keyfile_format::keyfile_t&& keyfile_data)
{
  auto res = m_keydata_map.insert(value_t(keyfile_data.keyname, keyfile_data));
  if(res.second == false)
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Keyfile with name \"${key_}\" is already exist", ("key_", keyfile_data.keyname));
  flush_keyfile(keyfile_data.keyname);
}

void keyfile_singleton::update(keyfile_format::keyfile_t&& keyfile_data)
{
  auto it = m_keydata_map.find(keyfile_data.keyname);
  if(it == m_keydata_map.end())
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Keyfile with name \"${key_}\" not found", ("key_", keyfile_data.keyname));
  it->second = keyfile_data;
  flush_keyfile(keyfile_data.keyname);
}

bool keyfile_singleton::is_exist(const std::string& key) const
{
  auto it = m_keydata_map.find(key);
  return it != m_keydata_map.end();
}

void keyfile_singleton::flush_keyfile(const std::string& key) const
{
  auto it = m_keydata_map.find(key);
  if( it == m_keydata_map.end() )
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Keyname is not exist (${key_})", ("key_", key));
  auto& keyfile_data = it->second;
  auto hash = dev::openssl::sha3(keyfile_data.keyinfo.public_key);
  auto filename = hash.hex().substr(0,16);
  filename += ".json";
  bfs::path filepath(std::string(KEY_DEFAULT_PATH_"/") +std::string(filename));
  auto fout = std::ofstream(filepath.c_str());
  if(!fout.is_open())
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile (${filename})", ("filename", filename));
  fout << fc_light::json::to_pretty_string(fc_light::variant(keyfile_data)) << std::endl;
}

void keyfile_singleton::flush_all() const
{
  auto curdir = bfs::current_path();
  auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
  std::for_each(m_keydata_map.begin(), m_keydata_map.end(), [first](const auto& keyfile_data){
    auto hash = dev::openssl::sha3(keyfile_data.second.keyinfo.public_key);
    auto filename = hash.hex().substr(0,16);
    filename += ".json";
    bfs::path filepath(std::string(KEY_DEFAULT_PATH_"/") +std::string(filename));
    auto fout = std::ofstream(filepath.c_str());
    if(!fout.is_open())
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Cannot open keyfile (${filename})", ("filename", filename));
    fout << fc_light::json::to_pretty_string(fc_light::variant(keyfile_data)) << std::endl;
  });
}