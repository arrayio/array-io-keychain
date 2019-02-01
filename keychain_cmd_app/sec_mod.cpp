//
// Created by roman on 5/14/18.
//

#include <fc_light/exception/exception.hpp>
#include <keychain_lib/secmod_parser_cmd.hpp>

#include "sec_mod.hpp"

using namespace keychain_app;

sec_mod_dummy::sec_mod_dummy()
{}

sec_mod_dummy::~sec_mod_dummy()
{}

std::string keychain_app::sec_mod_dummy::exec_cmd(const std::string& json_cmd) const
{
  auto& log = logger_singleton::instance();
  BOOST_LOG_SEV(log.lg, info) << "Send to secmod dummy:" + json_cmd;
  secmod_commands::secmod_parser_f parser;
  auto etype = parser(json_cmd);
  int unlock_time = 0;
  switch (etype)
  {
  case secmod_commands::events_te::create_key:
  case secmod_commands::events_te::sign_hex:
  case secmod_commands::events_te::sign_hash:
  case secmod_commands::events_te::remove_key:
  case secmod_commands::events_te::export_keys:
  case secmod_commands::events_te::import_keys:
  case secmod_commands::events_te::unlock:
  {
    std::string str = "blank";
    keychain_app::byte_seq_t pass(str.begin(), str.end());

    secmod_commands::secmod_resonse_common response;
    response.etype = secmod_commands::response_te::password;
    response.params = pass;
    return fc_light::json::to_pretty_string(response);
  }
  default:
  {
    FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", etype));
  }
  }

 
}

