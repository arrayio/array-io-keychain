//
// Created by roman on 4/25/18.
//

#ifndef KEYCHAINAPP_KEY_ENCRYPTOR_HPP
#define KEYCHAINAPP_KEY_ENCRYPTOR_HPP

#include <string>
#include <functional>
#include <openssl/ossl_typ.h>

#include "key_file_parser.hpp"

namespace keychain_app
{
class encryptor_singletone
{
  encryptor_singletone();
  ~encryptor_singletone();
public:
  static encryptor_singletone& instance();
  
  keyfile_format::encrypted_data encrypt_keydata(
    keyfile_format::cipher_etype etype,
    const std::string& key,
    const std::string& keydata);
  
  std::string decrypt_keydata(const std::string& key, keyfile_format::encrypted_data& enc_data);

private:
  EVP_CIPHER_CTX* m_ctx;
  static const EVP_CIPHER * get_cipher(keyfile_format::cipher_etype etype);
  static std::string random_string(size_t length = 32);
};

}

#endif //KEYCHAINAPP_KEY_ENCRYPTOR_HPP