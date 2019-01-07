//
// Created by roman on 4/25/18.
//

#ifndef KEYCHAINAPP_KEY_ENCRYPTOR_HPP
#define KEYCHAINAPP_KEY_ENCRYPTOR_HPP

#include <string>
#include <functional>
#include <openssl/ossl_typ.h>

#include <eth-crypto/crypto/Common.h>

#include "key_file_parser.hpp"

namespace keychain_app
{

using byte_seq_t = std::vector<char>;

class encryptor_singleton
{
  encryptor_singleton();
  ~encryptor_singleton();
public:
  static encryptor_singleton& instance();
  
  keyfile_format::encrypted_data encrypt_private_key(keyfile_format::cipher_etype etype, const byte_seq_t& key,  const dev::Secret& priv_key);
  dev::Secret decrypt_private_key(const byte_seq_t& key, keyfile_format::encrypted_data& enc_data);

private:
  EVP_CIPHER_CTX* m_ctx;
  static const EVP_CIPHER * get_cipher(keyfile_format::cipher_etype etype);
  static std::vector<uint8_t> random_string(size_t length = 16);
};

}

#endif //KEYCHAINAPP_KEY_ENCRYPTOR_HPP
