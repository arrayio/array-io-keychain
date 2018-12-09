//
// Created by roman on 4/20/18.
//

#include <fc_light/exception/exception.hpp>

#include "keychain_logger.hpp"
#include "secmod_protocol.hpp"
#include "secmod_parser_cmd.hpp"

namespace keychain_app
{

namespace secmod_commands
{

blockchain_secmod_te secmod_parser_f::operator()(const std::string& json)
{
  auto log = logger_singletone::instance();
  try
  {
    auto variant = fc_light::json::from_string(json);
    m_cmd = variant.as<secmod_command_common>();
  }
  catch (const fc_light::exception& exc){
    BOOST_LOG_SEV(log.lg, error) << "secmod common command parse error: " << exc.what();
    return blockchain_secmod_te::parse_error;
  }
}

std::string secmod_parser_f::to_pretty_string() const
{
  using ethereum_cmd = secmod_command<blockchain_secmod_te::ethereum>::type;
  using bitcoin_cmd = secmod_command<blockchain_secmod_te::bitcoin>::type;
  using ethereum_swap_cmd = secmod_command<blockchain_secmod_te::ethereum_swap>::type;
  using rawhash_cmd = secmod_command<blockchain_secmod_te::rawhash>::type;
  
  try
  {
    switch (m_cmd.blockchain)
    {
      case blockchain_secmod_te::ethereum:
      {
        auto cmd = m_cmd.data.as<ethereum_cmd>();
        return fc_light::json::to_pretty_string(fc_light::variant(cmd));
      }
      case blockchain_secmod_te::bitcoin:
      {
        auto cmd = m_cmd.data.as<bitcoin_cmd>();
        return fc_light::json::to_pretty_string(fc_light::variant(cmd));
      }
      case blockchain_secmod_te:: ethereum_swap:
      {
        auto cmd = m_cmd.data.as<ethereum_swap_cmd>();
        return fc_light::json::to_pretty_string(fc_light::variant(cmd));
      }
      case blockchain_secmod_te::rawhash:
      {
        auto cmd = m_cmd.data.as<rawhash_cmd>();
        return fc_light::json::to_pretty_string(fc_light::variant(cmd));
      }
      default:
        return std::string();
    }
  }
  catch (const fc_light::exception& exc)
  {
    auto log = logger_singletone::instance();
    BOOST_LOG_SEV(log.lg, error) << "secmod nested command error: " << exc.what();
    return std::string();
  }
  
}

}

}


