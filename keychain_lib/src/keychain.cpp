//
// Created by roman on 4/6/18.
//

#include "keychain.hpp"
#include "key_file_parser.hpp"
#include "keychain_commands.hpp"

#include <boost/hana/for_each.hpp>

#include <iostream>
#include <errno.h>

#include <boost/hana/size.hpp>

#include <boost/hana.hpp>


using namespace keychain_app;


keychain_base::keychain_base(){}

keychain_base::~keychain_base(){}


keychain_commands_singleton::keychain_commands_singleton()
{
  m_command_list.reserve(32);//TODO: it is may be possible to get size info from boost::hana::tuple
  hana::for_each(cmd_static_list, [&](auto val) {
    using value_type = decltype(val);
    constexpr auto const_val = static_cast<command_te>(value_type::value);
    m_command_list.push_back(command_ptr(new keychain_command<const_val>));
  });
}


keychain& keychain::instance()
{
  static keychain _instance;
  return _instance;
}


keychain::keychain()
  : keychain_base(),
    m_init_path(bfs::current_path())
{
  bfs::path key_dir(KEY_DEFAULT_PATH_);

  if(!bfs::exists(key_dir))
  {
    auto res = bfs::create_directories(key_dir);
    if(res == false)
      FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception,
                               "Can not create key directory, path = ${directory}", ("directory", key_dir.string()));
  }
}

keychain::~keychain()
{
  bfs::current_path(m_init_path);
}

std::string keychain::operator()(const fc_light::variant& command) {
  auto print_exception = [](int id, fc_light::exception &er) -> std::string
  {
    auto err_logs = er.get_log();
    std::vector<fc_light::log_context> log_contexts(err_logs.size());
    std::transform(err_logs.begin(), err_logs.end(), log_contexts.begin(), [](const auto& val){
      return val.get_context();
    });
    return fc_light::json::to_string(
      fc_light::variant(keychain_app::json_error(id, static_cast<fc_light::exception_code_te>(er.code()), er.to_string().c_str(), fc_light::variant(log_contexts))));
  };
  
  keychain_command_common cmd;
  try
  {
    cmd = command.as<keychain_command_common>();
  }
  catch( fc_light::bad_cast_exception& er)
  {
    fc_light::rpc_command_parse_exception er_(er.get_log());
    er_.append_log( FC_LIGHT_LOG_MESSAGE( error, "cannot parse command" ) );
    return print_exception(cmd.id, er_);
  }
  catch (fc_light::exception& er)
  {
    er.append_log(FC_LIGHT_LOG_MESSAGE(error, "unknown error"));
    return print_exception(cmd.id, er);
  }
  try
  {
    auto cmd_map = keychain_commands_singleton::instance();
    auto p_func = cmd_map[cmd.command];
    return (*p_func)(this, cmd.params, cmd.id);
  }
  catch( fc_light::exception& er ) {
    return print_exception(cmd.id, er);
  }
  catch( const std::exception& e ) {
    return fc_light::json::to_string(
      fc_light::variant(keychain_app::json_error(cmd.id, fc_light::std_exception_code, e.what())));
  }
  catch( ... ) {
    return fc_light::json::to_string(
      fc_light::variant(keychain_app::json_error(cmd.id, fc_light::unhandled_exception_code)));
   }
}

const keychain_commands_singleton& keychain_commands_singleton::instance()
{
  static const keychain_commands_singleton instance;
  return instance;
}

const keychain_commands_singleton::command_ptr keychain_commands_singleton::operator[](command_te cmd_type) const
{
  size_t ind = static_cast<size_t>(cmd_type);
  if (ind >= m_command_list.size())
    return m_command_list[0];
  return m_command_list[ind];
}

void keychain_app::secure_dlg_mod_base::connect(keychain_app::keychain_base &keychain_) const
{
  keychain_.get_passwd_trx.connect(std::bind(&secure_dlg_mod_base::get_passwd_trx, this, std::placeholders::_1));
  keychain_.get_passwd_on_create.connect(std::bind(&secure_dlg_mod_base::get_passwd_on_create, this, std::placeholders::_1));
  keychain_.get_passwd_unlock.connect(std::bind(&secure_dlg_mod_base::get_passwd_unlock, this, std::placeholders::_1, std::placeholders::_2));
  keychain_.print_mnemonic.connect(std::bind(&secure_dlg_mod_base::print_mnemonic, this, std::placeholders::_1));
}

void keychain_app::gui_mod_base::connect(keychain_app::keychain_base &keychain_) const
{
  keychain_.select_key.connect(std::bind(&gui_mod_base::select_key, this));
}