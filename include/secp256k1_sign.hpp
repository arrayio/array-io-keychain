//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_SECP256K1_SIGN_HPP
#define KEYCHAINAPP_SECP256K1_SIGN_HPP

#include "sign_define.hpp"

#include <secp256k1-zkp/secp256k1.h>

namespace keychain_app
{

bool is_sign_canonical( const signature_t & c );

static int extended_nonce_function(
    unsigned char *nonce32,
    const unsigned char *msg32,
    const unsigned char *key32,
    unsigned int attempt,
    const void *data )
{
  unsigned int* extra = (unsigned int*) data;
  (*extra)++;
  return secp256k1_nonce_function_default( nonce32, msg32, key32, *extra, nullptr );
}

const secp256k1_context_t* secp256k1_get_context();

struct secp256k1_ecdsa_sign_create
{
    static const fc::ecc::private_key empty_secret;
    static signature_t create_signature(const fc::ecc::private_key& key, const unit_list_t& list);
};

using secp256k1_sign_t = secp256k1_ecdsa_sign_create;

}

#endif //KEYCHAINAPP_SECP256K1_SIGN_HPP
