//
// Created by roman on 5/1/19.
//

#include "keychain_commands.hpp"
#include "keyfile_singletone.hpp"

using namespace keychain_app;

static const keyfile_singleton& keyfile_singleton::instance()
{
  static keyfile_singletone keyfile_map;
  return keyfile_map;
}

keyfile_singleton::keyfile_singleton()
{
  auto curdir = bfs::current_path();
  auto first = bfs::directory_iterator(bfs::path(KEY_DEFAULT_PATH_));
  std::for_each(first, bfs::directory_iterator(), [this](const auto& unit){
    auto j_keyfile = open_keyfile(unit.path().c_str());
    auto file_data = j_keyfile.as<keyfile_format::keyfile_t>();
    insert(value_t(file_data.keyname, file_data));
  });
}

keyfile_singleton::~keyfile_singletone()
{
  flush_all();
}

const keyfile_format::keyfile_t& keyfile_singleton::operator[](const std::string& key) const
{
  auto it = find(key);
  if( it == end() )
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Keyname is not exist (${key_})", ("key_", key));
  return it->second;
}

keyfile_format::keyfile_t& keyfile_singleton::operator[](const std::string& key)
{
  auto it = find(key);
  if( it == end() )
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Keyname is not exist (${key_})", ("key_", key));
  return it->second;
}

void keyfile_singleton::insert(keyfile_format::keyfile_t&& keyfile_data)
{
  auto res = keyfile_map_t::insert(value_t(keyfile_data.keyname, keyfile_data));
  if(res.second == false)
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Keyfile with name \"${key_}\" is already exist", ("key_", keyfile_data.keyname));
  flush_keyfile(keyfile_data.keyname);
}

bool keyfile_singleton::is_exist(const std::string& key) const
{
  auto it = find(key);
  return it != end();
}

void keyfile_singleton::flush_keyfile(const std::string& key) const
{
  auto it = find(key);
  if( it == end() )
    FC_LIGHT_THROW_EXCEPTION(fc_light::key_not_found_exception, "Keyname is not exist (${key_})", ("key_", key));
  auto& keyfile_data = *it;
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
  std::for_each(begin(), end(), [first](const auto& keyfile_data){
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