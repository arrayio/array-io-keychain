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

using signhex_event = secmod_event<events_te::sign_hex>::params_t;

std::string to_expert_mode_string(const signhex_event& signhex_event);

class secmod_parser_f
{
public:  
  template <events_te etype>
  typename secmod_event<etype>::params_t params() const
  {
    using params_t = typename secmod_event<etype>::params_t;
    try
    {
      return m_cmd.params.as<params_t>();
    }
    FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW(fc_light::parse_error_exception, error, "cannot parse secmod command params")
  }
  events_te operator()(const std::string& json);
  events_te operator()(const fc_light::variant& );
  secmod_command m_cmd;
};

class secmod_result_parser_f
{
public:
  template <response_te etype>
  typename secmod_response<etype>::params_t params() const
  {
    using params_t = typename secmod_response<etype>::params_t;
    try
    {
      return m_response.params.as<params_t>();
    }
    FC_LIGHT_CAPTURE_TYPECHANGE_AND_RETHROW(fc_light::parse_error_exception, error, "cannot parse secmod command params")
  }
  void params() const {}
  response_te operator()(const std::string& json);
  secmod_resonse_common m_response;
};

}

}


#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PARSER_HPP
