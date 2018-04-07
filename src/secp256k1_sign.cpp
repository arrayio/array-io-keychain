//
// Created by roman on 4/6/18.
//

#include <algorithm>
#include "secp256k1_sign.hpp"

using namespace keychain_app;

signature_t secp256k1_ecdsa_sign_create::create_signature(const fc::ecc::private_key &key, const keychain_app::unit_list &list) {
  assert(key.get_secret() != empty_secret);

  fc::sha256::encoder enc;
  std::for_each(list.begin(), list.end(),[&enc](
  const fc::sha256 &val){
    enc << val;
  });
  signature_t result;
  int recid;
  unsigned int counter = 0;
  do {
    assert(
        secp256k1_ecdsa_sign_compact(
            secp256k1_get_context(),
            (unsigned char *) enc.result().data(),
            (unsigned char *) result.begin() + 1,
            (unsigned char *) key.get_secret().data(),
            extended_nonce_function,
            &counter,
            &recid
        ));
  } while ( !is_sign_canonical(result));
  result.begin()[0] = 27 + 4 + recid;
  return result;
}

