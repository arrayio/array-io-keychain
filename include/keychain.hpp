//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_HPP
#define KEYCHAINAPP_KEYCHAIN_HPP

#include <functional>

#include <fc/crypto/elliptic.hpp>
#include <json/json.hpp>

#include "secp256k1_sign.hpp"

namespace keychain_app
{

using secp256k1_sign_t = secp256k1_ecdsa_sign_create;

template <typename sign_creatorT>
class keychain : public  sign_creatorT
{
public:
  void operator()(const nlohmann::json& cmd){
    //TODO: analysе json, detach key, chainid and transaction hash
    //TODO: call sign_creatorT::m_func(key, unit_list)
    //TODO: output to stdout json result
    // { "result": "afdasdf1234asd124123fw124ef"; }
  }
};

}

#endif //KEYCHAINAPP_KEYCHAIN_HPP
