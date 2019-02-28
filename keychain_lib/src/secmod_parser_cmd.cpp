//
// Created by roman on 8/12/18.
//

#include <fc_light/exception/exception.hpp>

#include "keychain_logger.hpp"
#include "secmod_protocol.hpp"
#include "secmod_parser_cmd.hpp"

#define PRETTY_PRINT(blockchain_type) \
  fc_light::json::to_pretty_string(fc_light::variant(signhex_event.get_trx_view<blockchain_type>().trx_info));

namespace keychain_app
{

namespace secmod_commands
{

events_te secmod_parser_f::operator()(const std::string& json)
{
  auto variant = fc_light::json::from_string(json);
  m_cmd = variant.as<secmod_command>();
  return m_cmd.etype;
}

events_te secmod_parser_f::operator()(const fc_light::variant& variant)
{
  m_cmd = variant.as<secmod_command>();
  return m_cmd.etype;
}

std::string to_expert_mode_string(const signhex_event& signhex_event)
{
  if (!signhex_event.is_parsed)
    return signhex_event.trx_view.as<std::string>();
  switch (signhex_event.blockchain)
  {
  case blockchain_secmod_te::unknown:
    return signhex_event.trx_view.as<std::string>();
  case blockchain_secmod_te::bitcoin:
    return PRETTY_PRINT(blockchain_secmod_te::bitcoin);
  case blockchain_secmod_te::ethereum:
    return PRETTY_PRINT(blockchain_secmod_te::ethereum);
  case blockchain_secmod_te::ethereum_swap:
    return PRETTY_PRINT(blockchain_secmod_te::ethereum_swap);
  default:
    FC_LIGHT_ASSERT(false);
    break;
  }
}


keychain_app::secmod_commands::response_te secmod_result_parser_f::operator()(const std::string& json)
{
  auto variant = fc_light::json::from_string(json);
  m_response = variant.as<secmod_response_common>();
  return m_response.etype;
}

}

}


