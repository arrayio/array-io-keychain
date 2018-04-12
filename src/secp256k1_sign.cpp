//
// Created by roman on 4/6/18.
//

#include <algorithm>
#include "secp256k1_sign.hpp"

using namespace keychain_app;

bool keychain_app::is_sign_canonical(const signature_t& c)
{
  return !(c.data[1] & 0x80)
         && !(c.data[1] == 0 && !(c.data[2] & 0x80))
         && !(c.data[33] & 0x80)
         && !(c.data[33] == 0 && !(c.data[34] & 0x80));
}

const secp256k1_context_t* keychain_app::secp256k1_get_context()
{
  static secp256k1_context_t* ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_RANGEPROOF | SECP256K1_CONTEXT_COMMIT );
  return ctx;
}

signature_t secp256k1_ecdsa_sign_create::create_signature(const fc::ecc::private_key &key, const keychain_app::unit_list_t &list) {
  assert(key.get_secret() != empty_secret);

  class unit_visitor: public boost::static_visitor<>
  {
  public:
      unit_visitor(fc::sha256::encoder& enc): m_enc(enc){}

      void operator()(const fc::sha256& val)
      {
        m_enc << val;
      }

      void operator()(const std::vector<char>& val)
      {
        return m_enc.write(static_cast<const char*>(val.data()), val.size());
      }
      fc::sha256::encoder& m_enc;
  };

  fc::sha256::encoder enc;
  unit_visitor var_visitor(enc);
  std::for_each(list.begin(), list.end(),[&var_visitor](const unit_t& val){
      boost::apply_visitor(var_visitor, val );
  });
  auto hash = enc.result();
  signature_t result;
  int recid;
  unsigned int counter = 0;
  do {
    assert(
        secp256k1_ecdsa_sign_compact(
            secp256k1_get_context(),
            (unsigned char *) hash.data(),
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

const fc::ecc::private_key secp256k1_ecdsa_sign_create::empty_secret;