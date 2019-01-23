// Created by roman on 8/12/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_SEC_MOD_PARSER_HPP
#define KEYCHAINAPP_KEYCHAIN_SEC_MOD_PARSER_HPP

#include <fc_light/exception/exception.hpp>
#include <fc_light/io/json.hpp>

#include "secmod_protocol.hpp"

namespace keychain_app
{

namespace secmod_commands
{

template <blockchain_secmod_te blockchain>
std::string to_expert_mode_string(const typename trx_view<blockchain>::type& params)
{
  return fc_light::json::to_pretty_string(fc_light::variant(params.trx_info));
}

class secmod_parser_f
{
public:
  
  using create_cmd = secmod_event<events_te::create_key>::params_t;
  using signhex_cmd = secmod_event<events_te::sign_hex>::params_t;
  using signhash_cmd = secmod_event<events_te::sign_hash>::params_t;
  using unlock_cmd = secmod_event<events_te::unlock>::params_t;
  using remove_cmd = secmod_event<events_te::remove_key>::params_t;
  using export_cmd = secmod_event<events_te::export_keys>::params_t;
  using import_cmd = secmod_event<events_te::import_keys>::params_t;
  using print_mnemonic_cmd = secmod_event<events_te::print_mnemonic>::params_t;
  
  events_te operator()(const std::string& json);
  events_te cmd_type() const;
  
  template <events_te etype>
  typename secmod_event<etype>::params_t params() const
  {
    using params_t = typename secmod_event<etype>::params_t;
    try
    {
      return m_cmd.params.as<params_t>();
    }
    FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW (fc_light::parse_error_exception, error, "cannot parse secmod command params")
  }
private:
  secmod_command m_cmd;
};

}

}


#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PARSER_HPP
