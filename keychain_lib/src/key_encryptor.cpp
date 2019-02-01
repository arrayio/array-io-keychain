//
// Created by roman on 4/25/18.
//

#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <chrono>
#include <random>
#include <type_traits>
#include "key_encryptor.hpp"
#include "keychain_commands.hpp"
#include <openssl/sha.h>

using namespace keychain_app;

encryptor_singleton::encryptor_singleton()
  : m_ctx(EVP_CIPHER_CTX_new())
{
  if (!m_ctx)
  {
    ERR_print_errors_fp(stderr);
    FC_LIGHT_THROW_EXCEPTION(fc_light::encryption_exception, "Can't create EVP cipher context");
  }
}

encryptor_singleton::~encryptor_singleton()
{
  EVP_CIPHER_CTX_free(m_ctx);
}

encryptor_singleton& encryptor_singleton::instance()
{
  static encryptor_singleton encryptor_instance;
  return encryptor_instance;
}

keyfile_format::encrypted_data encryptor_singleton::encrypt_private_key(keyfile_format::cipher_etype etype, const byte_seq_t& key, const dev::Secret& priv_key)
{
  dev::h512 plain_data;
  auto priv_key_unsec = priv_key.makeInsecure();
  auto prkey_hash = dev::openssl::sha3(priv_key);
  auto prkey_hash_unsec = prkey_hash.makeInsecure();
  auto it = std::copy(priv_key_unsec.begin(), priv_key_unsec.end(), plain_data.data());
  std::copy(prkey_hash_unsec.begin(), prkey_hash_unsec.end(), it);
  encrypt_data(etype, key, plain_data.data(), plain_data.size);
}

keyfile_format::encrypted_data encryptor_singleton::encrypt_data(keyfile_format::cipher_etype etype, const byte_seq_t& key, const unsigned char *in, int inl)
{
  keyfile_format::encrypted_data enc_data;
  
  int enc_length = 0;
  int length = 0;
  
  //NOTE: in certain cases, an error with EVP_DecryptInit_ex func may occur
  //In different cases, the function handles raw C key strings differently
  //depending on whether the string is allocated on the stack, heap or in the static memory area
  //I cannot figure out the exact reason what exactly is wrong with the key (it is need to debug asm function
  // to find out reason)
  //The solution (from lib/fc) is to create hash from password string and encrypt data on hash key
  auto iv = std::move(random_string());
  dev::openssl::sha3_512_encoder enc;//NOTE: use 512 hash for possible future 512 bit cipher algorithms
  enc.write(key.data(), key.size());
  auto key_hash = enc.result();
  
  std::vector<uint8_t > enc_byte_data(2048,0x00);//TODO: memory has been allocated with a stock
  //TODO: need to figure out how much memory need to allocate for encrypted data in dependence of cipher algo type
  
  if(1 != EVP_EncryptInit_ex(m_ctx, get_cipher(etype), NULL, key_hash.data(), iv.data()))
  {
    //TODO: need to print OpenSSL detailed error string
    FC_LIGHT_THROW_EXCEPTION(fc_light::encryption_exception, "Error: EVP_EncryptInit_ex");
  }
  iv.resize(EVP_CIPHER_CTX_iv_length(m_ctx));
  enc_data.iv = to_hex(iv.data(), iv.size());
  if(1 != EVP_EncryptUpdate(m_ctx, enc_byte_data.data(), &length, in, inl))
  {
    //TODO: need to print OpenSSL detailed error string
    FC_LIGHT_THROW_EXCEPTION(fc_light::encryption_exception, "Error: EVP_EncryptUpdate");
  }
  enc_length = length;
  if(1 != EVP_EncryptFinal_ex(m_ctx, enc_byte_data.data() + enc_length, &length))
  {
    //TODO: need to print OpenSSL detailed error string
    FC_LIGHT_THROW_EXCEPTION(fc_light::encryption_exception, "Error: EVP_EncryptFinal_ex");
  }
  enc_length += length;
  enc_byte_data.resize(enc_length);
  
  enc_data.enc_data.assign(std::move(to_hex(enc_byte_data.data(), enc_byte_data.size())));
  enc_data.cipher_type = etype;
  EVP_CIPHER_CTX_reset(m_ctx);
  return enc_data;
}

dev::Secret encryptor_singleton::decrypt_private_key(const byte_seq_t& key, keyfile_format::encrypted_data& data)
{
  int decr_length = 0;
  int length = 0;
  assert( (data.enc_data.size()&0x01) == 0);//hex data must have even num of symbols
  std::vector<uint8_t> enc_byte_data(data.enc_data.size()/2, 0x00);
  auto len = from_hex(data.enc_data, enc_byte_data.data(), enc_byte_data.size());
  assert(len == enc_byte_data.size());
  //TODO: need to figure out how much memory need to allocate for encrypted data in dependence of cipher algo type
  std::vector<uint8_t> decr_byte_data(2048,0x00);//TODO: memory has been allocated with a stock
  
  //NOTE: in certain cases, an error with EVP_DecryptInit_ex func may occur
  //In different cases, the function handles raw C key strings differently
  //depending on whether the string is allocated on the stack, heap or in the static memory area
  //I cannot figure out the exact reason what exactly is wrong with the key (it is need to debug asm function
  // to find out reason)
  //The solution (from lib/fc) is to create hash from password string and encrypt data on hash key
  dev::openssl::sha3_512_encoder enc;//NOTE: use 512 hash for possible future 512 bit cipher algorithms
  enc.write(key.data(), key.size());
  auto key_hash = enc.result();
  
  std::vector<uint8_t> iv(data.iv.size()/2, 0x00);
  from_hex(data.iv.data(), iv.data(), iv.size());
  
  if(1 != EVP_DecryptInit_ex(m_ctx, get_cipher(data.cipher_type), NULL, key_hash.data(), iv.data()))
  {
    //TODO: need to print OpenSSL detailed error string
    FC_LIGHT_THROW_EXCEPTION(fc_light::encryption_exception, "EVP_DecryptInit_ex");
  }
  if(1 != EVP_DecryptUpdate(m_ctx, decr_byte_data.data(), &length, enc_byte_data.data(), enc_byte_data.size()))
  {
    //TODO: need to print OpenSSL detailed error string
    FC_LIGHT_THROW_EXCEPTION(fc_light::encryption_exception, "EVP_DecryptUpdate");
  }
  decr_length = length;
  if(1 != EVP_DecryptFinal_ex(m_ctx, decr_byte_data.data() + decr_length, &length))
  {
    //TODO: need to print OpenSSL detailed error string
    FC_LIGHT_THROW_EXCEPTION(fc_light::privkey_invalid_unlock, "EVP_DecryptFinal_ex, cannot unlock private key, possible wrong password");
  }
  decr_length += length;
  decr_byte_data.resize(decr_length);
  FC_LIGHT_ASSERT(decr_byte_data.size() == dev::h512::size);
  dev::Secret priv_key(decr_byte_data, dev::FixedHash<dev::Secret::size>::AlignLeft);
  dev::Secret prkey_hash(decr_byte_data, dev::FixedHash<dev::Secret::size>::AlignRight);
  EVP_CIPHER_CTX_reset(m_ctx);
  auto eval_hash = dev::openssl::sha3(priv_key);
  if( prkey_hash != eval_hash)
    FC_LIGHT_THROW_EXCEPTION(fc_light::privkey_invalid_unlock, "cannot unlock private key, possible wrong password");
  return priv_key;
}

std::vector<uint8_t> encryptor_singleton::random_string(size_t length)
{
  static std::uniform_int_distribution<int> dist(0, 255);
  static std::mt19937 rand_dev(
    std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());
  
  std::vector<uint8_t>  res(length, 0x00);
  for (size_t i = 0; i < length; ++i) {
    res[i] = dist(rand_dev);
  }
  return res;
}

using namespace keyfile_format;

const EVP_CIPHER * encryptor_singleton::get_cipher(cipher_etype etype)
{
  switch (etype)
  {
    case cipher_etype::aes128: return EVP_aes_128_cbc();
    case cipher_etype::aes192: return EVP_aes_192_cbc();
    case cipher_etype::aes256: return EVP_aes_256_cbc();
    //TODO: need to add other cipher algorithms
    default:
      FC_LIGHT_THROW_EXCEPTION(fc_light::encryption_exception,
                               "Unsupported type of key cipher algorithm, cipher_etype = ${etype}", ("etype", etype));
  }
}