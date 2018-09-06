/*
 * sha3.h
 *
 *  Created on: Aug 31, 2018
 *  (c) 2018 array.io
 */

#include <ethereum/core/sha3.h>
#include <fc_light/exception/exception.hpp>

namespace dev {

sha3_encoder_base::sha3_encoder_base() {
  ctx = EVP_MD_CTX_create();
}

sha3_encoder_base::~sha3_encoder_base() {
  EVP_MD_CTX_destroy(ctx);
}

void sha3_encoder_base::write(const char *d, uint32_t dlen) {
  //TODO: implement error handling
  EVP_DigestUpdate(ctx, d, dlen);
}

void sha3_encoder_base::put(char c) {
  write(&c, 1);
}

h256 sha3_encoder_base::result() {
  h256 result;
  unsigned int digest_len;
  int sha256_size = EVP_MD_size(EVP_sha256());
  FC_LIGHT_ASSERT(result.size == sha256_size, "Invalid sha3_256 hash size");
  
  EVP_DigestFinal_ex(ctx, result.data(), &digest_len);

  FC_LIGHT_ASSERT(digest_len == result.size, "Invalid sha3_256 hash size has been written");
  
  return result;
}

void sha3_encoder_base::result(char *out, uint32_t dlen)
{
  unsigned int digest_len;
  int sha256_size = EVP_MD_size(EVP_sha256());
  FC_LIGHT_ASSERT(dlen == sha256_size, "Invalid sha3_256 hash size");
  EVP_DigestFinal_ex(ctx, (unsigned char*)out, &digest_len);
  FC_LIGHT_ASSERT(digest_len == dlen, "Invalid sha3_256 hash size has been written");
}

sha3_224_encoder::sha3_224_encoder()
{
  EVP_DigestInit_ex(ctx, EVP_sha224(), NULL);
}

sha3_224_encoder::~sha3_224_encoder(){}

sha3_256_encoder::sha3_256_encoder()
{
  EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
}

sha3_256_encoder::~sha3_256_encoder(){}

sha3_512_encoder::sha3_512_encoder()
{
  EVP_DigestInit_ex(ctx, EVP_sha512(), NULL);
}

sha3_512_encoder::~sha3_512_encoder(){}

bool sha3(bytesConstRef _input, bytesRef o_output) noexcept
{
  if (o_output.size() != 32)
    return false;
  sha3_256_encoder enc;
  enc.write((char*)_input.data(), _input.size());
  enc.result((char*)o_output.data(), o_output.size());
  return true;
}

}


