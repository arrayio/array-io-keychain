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
  
  using ethereum_cmd = secmod_command<blockchain_secmod_te::ethereum>::type;
  using bitcoin_cmd = secmod_command<blockchain_secmod_te::bitcoin>::type;
  using ethereum_swap_cmd = secmod_command<blockchain_secmod_te::ethereum_swap>::type;
  using rawhash_cmd = secmod_command<blockchain_secmod_te::rawhash>::type;
  
  blockchain_secmod_te operator()(const std::string& json);
  blockchain_secmod_te cmd_type() const;
  std::string keyname() const;
  int unlock_time() const;
  bool is_json() const;
  ethereum_cmd to_ethereum() const;
  bitcoin_cmd to_bitcoin() const;
  ethereum_swap_cmd to_ethereum_swap() const;
  rawhash_cmd to_rawhash() const;
  std::string to_raw_trx_string() const;
  
  std::string to_expert_mode_string() const;
  
private:
  secmod_command_common m_cmd;
};

}

}


#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PARSER_HPP
