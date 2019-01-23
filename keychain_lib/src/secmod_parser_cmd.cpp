//
// Created by roman on 8/12/18.
//

#include <fc_light/exception/exception.hpp>

#include "keychain_logger.hpp"
#include "secmod_protocol.hpp"
#include "secmod_parser_cmd.hpp"

namespace keychain_app
{

namespace secmod_commands
{

events_te secmod_parser_f::operator()(const std::string& json)
{
  auto variant = fc_light::json::from_string(json);
  m_cmd = variant.as<secmod_command>();
}

events_te secmod_parser_f::cmd_type() const
{
  return m_cmd.etype;
}

}

}


