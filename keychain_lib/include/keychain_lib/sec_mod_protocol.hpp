// Created by roman on 4/20/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP
#define KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP

namespace keychain_app
{

namespace sec_mod_commands
{

enum struct blockchain_secmod_te {
  unknown=0,
  ethereum,
  bitcoin,
  raw_hash,//NOTE: for sign_hash command
  ethereum_swap //HACK:
};


template<blockchain_secmod_te blockchain_type>
struct secmod_command
{
  using type = void;
};

struct sec_mod_command_common
{
  sec_mod_command_common(bool json_, blockchain_secmod_te blockchain_, fc_light::variant&& data_)
    : json(json_)
    , blockchain(blockchain_)
    , data(data_)
  {}
  bool json;
  blockchain_secmod_te blockchain;
  fc_light::variant data;
};

struct ethereum_trx_t
{
  ethereum_trx_t () : chainid(0) {}
  ethereum_trx_t(  std::string _nonce,
          std::string _gasPrice,
          std::string _gas,
          int _chainid,
          std::string _from,
          std::string _to,
          std::string _value):
    nonce(_nonce),
    gasPrice(_gasPrice),
    gas(_gas),
    chainid(_chainid),
    to(_to),
    value(_value){}
  std::string nonce, gasPrice, gas;
  int chainid;
  std::string to ,value;
};

template<>
struct secmod_command<blockchain_secmod_te::ethereum>
{
  struct sec_mod_command_ethereum {
    sec_mod_command_ethereum(std::string &&from_, ethereum_trx_t &&trx_)
      : from(from_), trx_info(trx_) {}
    
    std::string from;
    ethereum_trx_t trx_info;
  };
  
  using type = sec_mod_command_ethereum;
};

template<>
struct secmod_command<blockchain_secmod_te::ethereum_swap>
{
  struct sec_mod_command_swap {
    
    enum struct action_te {
      create_swap = 0,
      refund,
      withdraw
    };
    
    struct swap_t {
      action_te action;
      fc_light::variant params;
    };
    //TODO:  need one single type, because of integration with mac and windows platform
    struct swap_create {
      std::string hash;
      std::string address;
    };
    
    struct swap_refund {
      std::string address;
    };
    
    struct swap_withdraw {
      std::string secret;
      std::string address;
    };
    
    sec_mod_command_swap(std::string &&from_, ethereum_trx_t &&trx_, swap_t &&swap_info_)
      : from(from_), trx_info(trx_), swap_info(swap_info_) {}
    
    std::string from;
    ethereum_trx_t trx_info;
    swap_t swap_info;
  };
  
  using type = sec_mod_command_swap;
};

template<>
struct secmod_command<blockchain_secmod_te::bitcoin>
{
  struct sec_mod_command_bitcoin {
    sec_mod_command_bitcoin(std::string &&from_, bitcoin_transaction_t &&trx_)
      : from(from_), trx_info(trx_) {}
    
    std::string from;
    bitcoin_transaction_t trx_info;
  };
  
  using type = sec_mod_command_bitcoin;
};

template<>
struct secmod_command<blockchain_secmod_te::raw_hash>
{
  struct sec_mod_command_rawhash {
    sec_mod_command_rawhash(std::string &&from_, std::string &&hash_)
      : from(from_), hash(hash_) {}
    
    std::string from;
    std::string hash;
  };
  
  using type = sec_mod_command_rawhash;
};

}

}

FC_LIGHT_REFLECT_ENUM(keychain_app::sec_mod_commands::blockchain_secmod_te, (unknown)(ethereum)(bitcoin)(ethereum_swap))

FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::sec_mod_command_common, (json)(blockchain)(data))
FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::ethereum_trx_t, (nonce)(gasPrice)(gas)(chainid)(to)(value))
FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::ethereum>::type, (from)(trx_info))

FC_LIGHT_REFLECT_ENUM(
  keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::ethereum_swap>::type::action_te,
  (create_swap)(refund)(withdraw))

FC_LIGHT_REFLECT(
  keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::ethereum_swap>::type::swap_t,
  (action)(params))

FC_LIGHT_REFLECT(
  keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::ethereum_swap>::type::swap_create,
                 (hash)(address))

FC_LIGHT_REFLECT(
  keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::ethereum_swap>::type::swap_refund,
  (address))

FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::ethereum_swap>::type::swap_withdraw,
                 (secret)(address))

FC_LIGHT_REFLECT(
  keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::ethereum_swap>::type,
  (from)(trx_info)(swap_info))

FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::bitcoin>::type, (from)(trx_info))

FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::secmod_command<keychain_app::sec_mod_commands::blockchain_secmod_te::raw_hash>::type, (from)(hash))


#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP
