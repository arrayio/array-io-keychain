//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_SECP256K1_SIGN_HPP
#define KEYCHAINAPP_SECP256K1_SIGN_HPP

#include "sign_define.hpp"

namespace keychain_app
{

struct secp256k1_ecdsa_sign_create
{
    static signature_t create_signature(const fc::ecc::private_key& key, const unit_list& list);
};

}

#endif //KEYCHAINAPP_SECP256K1_SIGN_HPP
