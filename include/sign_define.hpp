//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_SIGN_DEFINE_HPP
#define KEYCHAINAPP_SIGN_DEFINE_HPP

#include <fc/crypto/elliptic.hpp>
#include <boost/variant.hpp>

namespace keychain_app
{

using signature_t = fc::ecc::compact_signature;
using unit_t = boost::variant< fc::sha256, std::vector<char> >;
using unit_list_t = std::vector< unit_t >;
using secp256_private_key = fc::ecc::private_key;
static const char* KEY_DEFAULT_PATH = "./key_data";

}

#endif //KEYCHAINAPP_SIGN_DEFINE_HPP
