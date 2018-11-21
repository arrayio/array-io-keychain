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
  : keychain_base()
  , m_init_path(bfs::current_path())
{
  binary_dir =  bfs::absolute("").string();

  std::string dir(default_key_dir);
  bfs::path key_dir(dir);

  key_dir +=  "/key_data";

  if(!bfs::exists(key_dir))
  {
      auto res = bfs::create_directories(key_dir);
      if(res == false)
          throw std::runtime_error("Error: can not create key directory");
  }

  get_passwd_trx_raw.connect(std::bind(&secure_dlg_mod_base::get_passwd_trx_raw, secure_dlg,
          std::placeholders::_1,  std::placeholders::_2));
  get_passwd_on_create.connect(std::bind(&secure_dlg_mod_base::get_passwd_on_create, secure_dlg,
          std::placeholders::_1));
  print_mnemonic.connect(std::bind(&secure_dlg_mod_base::print_mnemonic, secure_dlg,
          std::placeholders::_1));
  bfs::current_path(key_dir);
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
    std::cerr << fc_light::json::to_string(fc_light::variant(json_error(0, exc.to_detail_string().c_str()))) << std::endl;
	return fc_light::json::to_string(fc_light::variant(json_error(0, exc.what())));
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

