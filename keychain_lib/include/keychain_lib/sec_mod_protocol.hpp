// Created by roman on 4/20/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP
#define KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP

namespace keychain_app
{

namespace sec_mod_commands
{

struct tx_common
{
  tx_common(bool json_, blockchain_te blockchain_):
    json(json_), blockchain(blockchain_){}
  tx_common(bool json_, blockchain_te blockchain_, std::string raw):
    json(json_), blockchain(blockchain_){data = fc_light::variant(raw);}
  bool json;
  blockchain_te blockchain;
  fc_light::variant data;
};

struct eth_tx : tx_common
{
  eth_tx( blockchain_te blockchain,
          std::string nonce,
          std::string gasPrice,
          std::string gas,
          int chainid,
          std::string from,
          std::string to,
          std::string value
  ):
    tx_common(true, blockchain),
    trx(nonce, gasPrice, gas, chainid, from, to, value)
  { data  = fc_light::variant(trx); }
  
  struct trx_t
  {
    trx_t () {}
    trx_t(  std::string _nonce,
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
      from(_from),
      to(_to),
      value(_value){}
    std::string nonce, gasPrice, gas;
    int chainid;
    std::string from, to ,value;
  } trx;
};

struct tx_swap_common
{
  tx_swap_common(bool json_, blockchain_te blockchain_):
    json(json_), blockchain(blockchain_){}
  
  bool json;
  blockchain_te blockchain;
  fc_light::variant data;
  fc_light::variant swap;
};


struct eth_swap_tx : tx_swap_common
{
  eth_swap_tx( blockchain_te blockchain,
               std::string nonce,
               std::string gasPrice,
               std::string gas,
               int chainid,
               std::string from,
               std::string to,
               std::string value,
               fc_light::variant sw
  ):
    tx_swap_common(true, blockchain),
    trx(nonce, gasPrice, gas, chainid, from, to, value)
  { data  = fc_light::variant(trx);
    swap = sw;
  }
  
  struct trx_t
  {
    trx_t () {}
    trx_t(  std::string _nonce,
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
      from(_from),
      to(_to),
      value(_value){}
    std::string nonce, gasPrice, gas;
    int chainid;
    std::string from, to ,value;
  } trx;
};


struct createSwap_tx
{
  createSwap_tx( std::string hash, std::string addr):   params(hash, addr){}
  struct params_t
  {
    params_t(std::string  hash_,  std::string addr_):
      action("createSwap"), hash(hash_), address(addr_){}
    std::string action, hash, address;
  } params;
};

struct refund_tx
{
  refund_tx( std::string addr): params(addr){}
  struct params_t
  {
    params_t(std::string addr): action("refund"), address(addr){}
    std::string action, address;
  } params;
};

struct Withdraw_tx
{
  Withdraw_tx( std::string secret, std::string addr): params(secret, addr){}
  struct params_t
  {
    params_t(std::string  sec,  std::string addr): action("Withdraw"), secret(sec), address(addr){}
    std::string action, secret, address;
  } params;
};

}

}

FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::tx_common, (json)(blockchain)(data))
FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::eth_tx::trx_t, (nonce)(gasPrice)(gas)(chainid)(from)(to)(value))

FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::tx_swap_common, (json)(blockchain)(data)(swap))
FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::eth_swap_tx::trx_t, (nonce)(gasPrice)(gas)(chainid)(from)(to)(value))

FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::createSwap_tx::params_t, (action)(hash)(address))
FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::refund_tx::params_t, (action)(address))
FC_LIGHT_REFLECT(keychain_app::sec_mod_commands::Withdraw_tx::params_t, (action)(secret)(address))

#endif //KEYCHAINAPP_KEYCHAIN_SEC_MOD_PROTOCOL_HPP
