/*
 * sha3.h
 *
 *  Created on: Aug 31, 2018
 *  (c) 2018 array.io
 */

#include <ethereum/core/sha3.h>
#include <fc_light/exception/exception.hpp>

namespace dev {

sha3_224_encoder::sha3_224_encoder()
  :sha3_encoder_base(EVP_sha224)
{
  EVP_DigestInit_ex(ctx, EVP_sha224(), NULL);
}

sha3_224_encoder::~sha3_224_encoder(){}

sha3_256_encoder::sha3_256_encoder()
  :sha3_encoder_base(EVP_sha256)
{
  EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
}

sha3_256_encoder::~sha3_256_encoder(){}

sha3_512_encoder::sha3_512_encoder()
  :sha3_encoder_base(EVP_sha512)
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


