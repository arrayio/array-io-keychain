// Created by roman on 8/12/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_SEC_MOD_PARSER_HPP
#define KEYCHAINAPP_KEYCHAIN_SEC_MOD_PARSER_HPP

#include "secmod_protocol.hpp"

namespace keychain_app
{

namespace secmod_commands
{

class secmod_parser_f
{
public:
  
  using create_cmd = secmod_event<events_te::create_key>::params;
  using signhex_cmd = secmod_event<events_te::sign_hex>::params;
  using signhash_cmd = secmod_event<events_te::sign_hash>::params;
  using unlock_cmd = secmod_event<events_te::unlock>::params;
  using remove_cmd = secmod_event<events_te::remove_key>::params;
  using export_cmd = secmod_event<events_te::export_keys>::params;
  using import_cmd = secmod_event<events_te::import_keys>::params;
  using print_mnemonic_cmd = secmod_event<events_te::print_mnemonic>::params;
  
  events_te operator()(const std::string& json);
  events_te cmd_type() const;
  
  template <events_te etype>
  secmod_event<etype>::params params() const;
private:
  secmod_command m_cmd;
};

}

}


#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PARSER_HPP
