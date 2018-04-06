//
// Created by roman on 4/6/18.
//

#include "secp256k1_sign.hpp"

signature_t sign_creator::create_signature(const fc::ecc::private_key &key, const keychain_app::unit_list &list)
{
  assert( key != empty_priv );

  fc::sha256::encoder enc;
  std::for_each(list.begin(), list.end(). [&enc](const fc::sha256& val){
    enc << val;
  });
  signature_t result;
  int recid;
  unsigned int counter = 0;
  do
  {
    assert(
        secp256k1_ecdsa_sign_compact(
            detail::_get_context(),
            (unsigned char*) digest.data(),
            (unsigned char*) result.begin() + 1,
            (unsigned char*) _key.data(),
            extended_nonce_function,
            &counter,
            &recid
        ));
  } while( require_canonical && !public_key::is_canonical( result ) );
  result.begin()[0] = 27 + 4 + recid;
  return result;
}
