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

namespace keychain_app {

namespace secmod_commands {

enum struct blockchain_secmod_te {
  unknown = 0,
  ethereum,
  bitcoin,
  ethereum_swap, //HACK:
  parse_error = -1
};

template<blockchain_secmod_te blockchain_type>
struct trx_view {
  using type = void;
};

struct ethereum_trx_t {
  ethereum_trx_t() : chainid(0) {}
  
  std::string nonce, gasPrice, gas;
  int chainid;
  std::string to, value, data;
};

template<>
struct trx_view<blockchain_secmod_te::ethereum> {
  struct secmod_command_ethereum {
    secmod_command_ethereum() {}
    
    secmod_command_ethereum(std::string &&from_, ethereum_trx_t &&trx_)
      : from(from_), trx_info(trx_) {}
    
    std::string from;
    ethereum_trx_t trx_info;
  };
  
  using type = secmod_command_ethereum;
};

template<>
struct trx_view<blockchain_secmod_te::ethereum_swap> {
  struct secmod_command_swap {
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
    
    secmod_command_swap() {}
    
    secmod_command_swap(std::string &&from_, ethereum_trx_t &&trx_, swap_t &&swap_info_)
      : from(from_), trx_info(trx_), swap_info(swap_info_) {}
    
    std::string from;
    ethereum_trx_t trx_info;
    swap_t swap_info;
  };
  
  using type = secmod_command_swap;
};

template<>
struct trx_view<blockchain_secmod_te::bitcoin> {
  struct secmod_command_bitcoin {
    secmod_command_bitcoin() {}
    
    secmod_command_bitcoin(std::string &&from_, bitcoin_transaction_t &&trx_)
      : from(from_), trx_info(trx_) {}
    
    std::string from;
    bitcoin_transaction_t trx_info;
  };
  
  using type = secmod_command_bitcoin;
};

enum struct events_te {
  unknown = 0,
  create_key,
  sign_hex,
  sign_hash,
  unlock,
  remove_key,
  export_keys,
  import_keys,
  print_mnemonic
};

template<events_te event>
struct secmod_event
{
  using params_t = void;
};

template<>
struct secmod_event<events_te::create_key>
{
  struct params {
    std::string keyname;
  };
  using params_t = params;
};

template<>
struct secmod_event<events_te::sign_hex>
{
  struct params {
    params() : is_parsed(false), unlock_time(0) {}
    
    std::string keyname;
    bool is_parsed;
    blockchain_secmod_te blockchain;
    int unlock_time;
    fc_light::variant trx_view;
  };
  
  using params_t = params;
};

template<>
struct secmod_event<events_te::sign_hash>
{
  struct params {
    std::string keyname;
    std::string from;
    std::string hash;
  };
  using params_t = params;
};

template<>
struct secmod_event<events_te::unlock>
{
  struct params
  {
    params(): unlock_time(0){}
    std::string keyname;
    int unlock_time;
  };
  
  using params_t = params;
};

template<>
struct secmod_event<events_te::remove_key>
{
  struct params {
    std::string keyname;
  };
  using params_t = params;
};

template<>
struct secmod_event<events_te::export_keys>
{
  using params_t = void;
};

template<>
struct secmod_event<events_te::import_keys>
{
  using params_t = void;
};

template<>
struct secmod_event<events_te::print_mnemonic>
{
  using params_t = void; //TODO: will be implemented in future
};

}

}

FC_LIGHT_REFLECT_ENUM(keychain_app::secmod_commands::blockchain_secmod_te, (unknown)(ethereum)(bitcoin)(ethereum_swap)(parse_error))
FC_LIGHT_REFLECT_ENUM(keychain_app::secmod_commands::events_te,
                      (unknown)(create_key)(sign_hex)(sign_hash)(unlock)(remove_key)(export_keys)(import_keys)(print_mnemonic))

FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::create_key>::params_t, (keyname))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::sign_hex>::params_t, (keyname)(is_parsed)(blockchain)(unlock_time)(trx_view))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::sign_hash>::params_t, (keyname)(from)(hash))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::unlock>::params_t, (keyname)(unlock_time))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::remove_key>::params_t, (keyname))

FC_LIGHT_REFLECT(keychain_app::secmod_commands::ethereum_trx_t, (nonce)(gasPrice)(gas)(to)(value)(data)(chainid))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::trx_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum>::type, (from)(trx_info))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::trx_view<keychain_app::secmod_commands::blockchain_secmod_te::bitcoin>::type, (from)(trx_info))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::trx_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type, (from)(trx_info)(swap_info))

FC_LIGHT_REFLECT_ENUM(
  keychain_app::secmod_commands::trx_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type::action_te,
  (create_swap)(refund)(withdraw))

FC_LIGHT_REFLECT(
  keychain_app::secmod_commands::trx_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type::swap_t,
  (action)(hash)(address)(secret))

#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP
