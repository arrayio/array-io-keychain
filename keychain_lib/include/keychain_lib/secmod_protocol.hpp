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

using byte_seq_t = std::vector<char>;

namespace secmod_commands {

enum struct blockchain_secmod_te {
  unknown = 0,
  ethereum,
  bitcoin,
  ethereum_swap, //HACK:
};

template<blockchain_secmod_te blockchain_type>
struct transaction_view
{
  using type = std::string; // if transaction cannot be parsed
};

struct ethereum_trx_t {
  ethereum_trx_t() : chainid(0) {}
  
  std::string nonce, gasPrice, gas;
  int chainid;
  std::string to, value, data;
};

template<>
struct transaction_view<blockchain_secmod_te::ethereum> {
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
struct transaction_view<blockchain_secmod_te::ethereum_swap> {
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
struct transaction_view<blockchain_secmod_te::bitcoin> {
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
  sign_trx,
  sign_hash,
  unlock,
  edit_key,
  remove_key,
  export_keys,
  import_keys,
  print_mnemonic,
  last
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
    params(): keyname(""){}
    params(params&& a): keyname(a.keyname){}
    std::string keyname;
  };
  using params_t = params;
};

template<>
struct secmod_event<events_te::sign_trx>
{
  struct params {
    params() : is_parsed(false), no_password(false), unlock_time(0) {}
    params(params&& a ):is_parsed(a.is_parsed), no_password(a.no_password), keyname(a.keyname),
        blockchain(a.blockchain), unlock_time(a.unlock_time), trx_view(a.trx_view) {}

    bool is_parsed;
    bool no_password;
    std::string keyname;
    blockchain_secmod_te blockchain;
    int unlock_time;
    fc_light::variant trx_view;

    template <blockchain_secmod_te blockchain_type>
    typename transaction_view<blockchain_type>::type get_trx_view() const
    {
      using return_t = typename transaction_view<blockchain_type>::type;
      return trx_view.as<return_t>();
    }
  };
  
  using params_t = params;
};

template<>
struct secmod_event<events_te::sign_hash>
{
  struct params {
    params(): no_password(false), keyname(""), from(""), hash(""){};
    params(params&& a): no_password(a.no_password), keyname(a.keyname), from(a.from), hash(a.hash){}
    bool no_password = false;
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
    params(): no_password(false), unlock_time(0){}
    params(params&& a): no_password(a.no_password), keyname(a.keyname), unlock_time(a.unlock_time){}
    bool no_password;
    std::string keyname;
    int unlock_time;
  };
  
  using params_t = params;
};

template<>
struct secmod_event<events_te::edit_key>
{
  struct params
  {
    params() : unlock_time(0) {}
    params(params&& a): keyname(a.keyname), unlock_time(a.unlock_time){}
    std::string keyname;
    int unlock_time;
  };

  using params_t = params;
};

template<>
struct secmod_event<events_te::remove_key>
{
  struct params {
    params(params &&a): keyname(a.keyname){}
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

struct secmod_command
{
  secmod_command() : etype(secmod_commands::events_te::unknown) {}
  events_te etype;
  fc_light::variant params;
};  

enum struct response_te
{
  null = 0,
  password,
  boolean,
  canceled
};

template <response_te response_type>
struct secmod_response
{
  using params_t = void;
};

template <>
struct secmod_response<response_te::password>
{
  using params_t = byte_seq_t;
};

template <>
struct secmod_response<response_te::boolean>
{
  using params_t = bool;
};

struct secmod_response_common
{
  secmod_response_common() : etype(secmod_commands::response_te::null) {}
  response_te etype;
  fc_light::variant params;
};

}

}

FC_LIGHT_REFLECT_ENUM(keychain_app::secmod_commands::blockchain_secmod_te, (unknown)(ethereum)(bitcoin)(ethereum_swap))
FC_LIGHT_REFLECT_ENUM(keychain_app::secmod_commands::events_te,
                      (unknown)(create_key)(sign_trx)(sign_hash)(unlock)(edit_key)(remove_key)(export_keys)(import_keys)(print_mnemonic))
FC_LIGHT_REFLECT_ENUM(keychain_app::secmod_commands::response_te, (null)(password)(boolean)(canceled))


FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::create_key>::params_t, (keyname))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::sign_trx>::params_t, (is_parsed)(no_password)(keyname)(blockchain)(unlock_time)(trx_view))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::sign_hash>::params_t, (no_password)(keyname)(from)(hash))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::unlock>::params_t, (no_password)(keyname)(unlock_time))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::edit_key>::params_t, (keyname)(unlock_time))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_event<keychain_app::secmod_commands::events_te::remove_key>::params_t, (keyname))

FC_LIGHT_REFLECT(keychain_app::secmod_commands::ethereum_trx_t, (nonce)(gasPrice)(gas)(to)(value)(data)(chainid))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::transaction_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum>::type, (from)(trx_info))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::transaction_view<keychain_app::secmod_commands::blockchain_secmod_te::bitcoin>::type, (from)(trx_info))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::transaction_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type, (from)(trx_info)(swap_info))

FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_command, (etype)(params))
FC_LIGHT_REFLECT(keychain_app::secmod_commands::secmod_response_common, (etype)(params))

FC_LIGHT_REFLECT_ENUM(
  keychain_app::secmod_commands::transaction_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type::action_te,
  (create_swap)(refund)(withdraw))

FC_LIGHT_REFLECT(
  keychain_app::secmod_commands::transaction_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::type::swap_t,
  (action)(hash)(address)(secret))

#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP
