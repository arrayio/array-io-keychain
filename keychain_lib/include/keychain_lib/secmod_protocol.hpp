// Created by roman on 8/12/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP
#define KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP

#include <string>
#include <fc_light/variant.hpp>
#include <fc_light/io/json.hpp>
#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>
#include "bitcoin_transaction.hpp"

namespace keychain_app
{

namespace secmod_commands
{

enum struct blockchain_secmod_te {
  unknown = 0,
  ethereum,
  bitcoin,
  rawhash, //NOTE: for sign_hash command
  ethereum_swap, //HACK:
  parse_error = -1
};

template<blockchain_secmod_te blockchain_type>
struct secmod_command
{
  using type = void;
};

struct secmod_command_common
{
  secmod_command_common(bool json_, blockchain_secmod_te blockchain_, const std::string& keyname_, fc_light::variant&& data_, int unlock_time_ = 0)
    : json(json_)
    , blockchain(blockchain_)
    , unlock_time(unlock_time_)
	, keyname(keyname_)
    , data(data_)
  {}
  
  secmod_command_common()
    : json(false)
    , blockchain(blockchain_secmod_te::unknown)
    , unlock_time(0)
  {}
  
  bool json;
  blockchain_secmod_te blockchain;
  int unlock_time;
  std::string keyname;
  fc_light::variant data;
};

struct ethereum_trx_t
{
  ethereum_trx_t () : chainid(0) {}
  std::string nonce, gasPrice, gas;
  int chainid;
  std::string to ,value, data;
};

template<>
struct secmod_command<blockchain_secmod_te::ethereum>
{
  struct secmod_command_ethereum
  {
    secmod_command_ethereum(){}
    secmod_command_ethereum(std::string &&from_, ethereum_trx_t &&trx_)
      : from(from_), trx_info(trx_) {}
    
    std::string from;
    ethereum_trx_t trx_info;
  };
  
  using type = secmod_command_ethereum;
};

template<>
struct secmod_command<blockchain_secmod_te::ethereum_swap>
{
  struct secmod_command_swap
  {
    enum struct action_te {
      create_swap = 0,
      refund,
      withdraw
    };
    
    struct swap_t {
      action_te action;
      std::string hash;
      std::string address;
      std::string secret;
    };
    
    secmod_command_swap(){}
    secmod_command_swap(std::string &&from_, ethereum_trx_t &&trx_, swap_t &&swap_info_)
      : from(from_), trx_info(trx_), swap_info(swap_info_) {}
    
    std::string from;
    ethereum_trx_t trx_info;
    swap_t swap_info;
  };
  
  using type = secmod_command_swap;
};

template<>
struct secmod_command<blockchain_secmod_te::bitcoin>
{
  struct secmod_command_bitcoin
  {
    secmod_command_bitcoin(){}
    secmod_command_bitcoin(std::string &&from_, bitcoin_transaction_t &&trx_)
      : from(from_), trx_info(trx_) {}
    
    std::string from;
    bitcoin_transaction_t trx_info;
  };
  
  using type = secmod_command_bitcoin;
};

template<>
struct secmod_command<blockchain_secmod_te::rawhash>
{
  struct secmod_command_rawhash
  {
    secmod_command_rawhash(){}
    secmod_command_rawhash(std::string &&from_, std::string hash_)
      : from(from_), hash(hash_) {}
    
    std::string from;
    std::string hash;
  };
  
  using type = secmod_command_rawhash;
};

}

}

FC_LIGHT_REFLECT_ENUM(keychain_app::secmod_commands::blockchain_secmod_te, (unknown)(ethereum)(bitcoin)(rawhash)(ethereum_swap)(parse_error))

FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_command_common, (json)(blockchain)(keyname)(data)(unlock_time))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::ethereum_trx_t, (nonce)(gasPrice)(gas)(to)(value)(data)(chainid))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::ethereum>::type, (from)(trx_info))

FC_LIGHT_REFLECT_ENUM(
  keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type::action_te,
  (create_swap)(refund)(withdraw))

FC_LIGHT_REFLECT(
  keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type::swap_t,
  (action)(hash)(address)(secret))

FC_LIGHT_REFLECT(
  keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type,
  (from)(trx_info)(swap_info))

FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::bitcoin>::type, (from)(trx_info))

FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::rawhash>::type, (from)(hash))


#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP
