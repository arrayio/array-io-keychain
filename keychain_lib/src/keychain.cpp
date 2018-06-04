//
// Created by roman on 4/6/18.
//

#include "keychain.hpp"
#include "key_file_parser.hpp"
#include "keychain_commands.hpp"
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/elliptic.hpp>

//TODO: it is unclear
//      why if we do not include this file we cannot compile reflect parser for keychain_app::keyfile_format::key_file
#include <graphene/chain/protocol/transaction.hpp>

#include <boost/hana/for_each.hpp>

#include <iostream>
#include <errno.h>

#include <boost/hana/size.hpp>

#include <boost/hana.hpp>

using namespace keychain_app;

keychain_commands_singletone::keychain_commands_singletone()
{
  m_command_list.reserve(32);//TODO: it is may be possible to get size info from boost::hana::tuple
  hana::for_each(cmd_static_list, [&](auto val) {
    using value_type = decltype(val);
    constexpr auto const_val = static_cast<keychain_command_type>(value_type::value);
    m_command_list.push_back(command_ptr(new keychain_command<const_val>()));
  });
}

keychain::keychain(std::string&& uid_hash_, const char* default_key_dir)
  : keychain_base(std::move(uid_hash_))
  , m_init_path(bfs::current_path())
{
  std::string user_dir(default_key_dir);
  bfs::path path_(user_dir);
  if(!bfs::exists(path_))
  {
    auto res = bfs::create_directory(path_);
    if(res == false)
      throw std::runtime_error("Error: can not create default key directory");
  }
  user_dir += "/";
  user_dir += uid_hash;
  path_ = bfs::path(user_dir);
  if(!bfs::exists(path_))
  {
    auto res = bfs::create_directory(path_);
    if(res == false)
      throw std::runtime_error("Error: can not create default key directory");
  }
  bfs::current_path(path_);
}

keychain::~keychain()
{
  bfs::current_path(m_init_path);
}

void keychain::operator()(const fc::variant& command) {
  try
  {
    auto cmd = command.as<keychain_command_common>();
    auto cmd_map = keychain_commands_singletone::instance();
    auto p_func = cmd_map[cmd.command];
    (*p_func)(this, cmd.params, cmd.id);
  }
  catch(fc::exception& exc)
  {
    std::cout << fc::json::to_pretty_string(fc::variant(json_error(0, exc.what()))) << std::endl;
    std::cerr << fc::json::to_pretty_string(fc::variant(json_error(0, exc.to_detail_string().c_str()))) << std::endl;
  }
}

const keychain_commands_singletone& keychain_commands_singletone::instance()
{
  static const keychain_commands_singletone instance;
  return instance;
}

const keychain_commands_singletone::command_ptr keychain_commands_singletone::operator[](keychain_command_type cmd_type) const
{
  size_t ind = static_cast<size_t>(cmd_type);
  if (ind >= m_command_list.size())
    return m_command_list[0];
  return m_command_list[ind];
}

