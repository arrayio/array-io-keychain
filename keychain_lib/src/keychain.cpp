//
// Created by roman on 4/6/18.
//

#include "keychain.hpp"
#include "key_file_parser.hpp"
#include "keychain_commands.hpp"

//TODO: it is unclear
//      why if we do not include this file we cannot compile reflect parser for keychain_app::keyfile_format::key_file

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
    constexpr auto const_val = static_cast<command_te>(value_type::value);
    m_command_list.push_back(command_ptr(new keychain_command<const_val>));
  });
}


keychain& keychain::instance(const secure_dlg_mod_base* secure_dlg )
{
  static keychain _instance (secure_dlg);

  return _instance;
}


keychain::keychain(const secure_dlg_mod_base* secure_dlg, const char* default_key_dir)
  : keychain_base(std::move(secure_dlg->get_uid()))
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

  get_passwd_trx_raw.connect(std::bind(&secure_dlg_mod_base::get_passwd_trx_raw, secure_dlg, std::placeholders::_1));
  get_passwd_on_create.connect(std::bind(&secure_dlg_mod_base::get_passwd_on_create, secure_dlg));
  print_mnemonic.connect(std::bind(&secure_dlg_mod_base::print_mnemonic, secure_dlg, std::placeholders::_1));

}

keychain::~keychain()
{
  bfs::current_path(m_init_path);
}

std::string keychain::operator()(const fc_light::variant& command) {
  try
  {
    auto cmd = command.as<keychain_command_common>();
    auto cmd_map = keychain_commands_singletone::instance();
    auto p_func = cmd_map[cmd.command];
    return (*p_func)(this, cmd.params, cmd.id);
  }
  catch(fc_light::exception& exc)
  {
    std::cerr << fc_light::json::to_pretty_string(fc_light::variant(json_error(0, exc.to_detail_string().c_str()))) << std::endl;
	return fc_light::json::to_pretty_string(fc_light::variant(json_error(0, exc.what())));
  }
}

const keychain_commands_singletone& keychain_commands_singletone::instance()
{
  static const keychain_commands_singletone instance;
  return instance;
}

const keychain_commands_singletone::command_ptr keychain_commands_singletone::operator[](command_te cmd_type) const
{
  size_t ind = static_cast<size_t>(cmd_type);
  if (ind >= m_command_list.size())
    return m_command_list[0];
  return m_command_list[ind];
}

