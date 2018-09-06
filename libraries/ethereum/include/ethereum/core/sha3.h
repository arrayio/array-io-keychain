/*
 * sha3.h
 *
 *  Created on: Aug 31, 2018
 *  (c) 2018 array.io
 */

#pragma once

#include "FixedHash.h"

#include <cstdint>
#include <cstring>
#include <fc_light/fwd.hpp>
#include <fc_light/string.hpp>
#include <fc_light/io/raw_fwd.hpp>

#include <openssl/evp.h>

namespace dev
{

class sha3_encoder_base {
public:
  sha3_encoder_base();
  virtual ~sha3_encoder_base();
  
  void write(const char *d, uint32_t dlen);
  void put(char c);
  h256 result();
  void result(char *out, uint32_t dlen);
protected:
  EVP_MD_CTX* ctx;
};

class sha3_224_encoder: public sha3_encoder_base
{
public:
  sha3_224_encoder();
  virtual ~sha3_224_encoder();
};

class sha3_256_encoder: public sha3_encoder_base
{
public:
  sha3_256_encoder();
  virtual ~sha3_256_encoder();
};

class sha3_512_encoder: public sha3_encoder_base
{
public:
  sha3_512_encoder();
  virtual ~sha3_512_encoder();
};

// SHA-3 convenience routines.

/// Calculate SHA3-256 hash of the given input and load it into the given output.
/// @returns false if o_output.size() != 32.
bool sha3(bytesConstRef _input, bytesRef o_output) noexcept;

/// Calculate SHA3-256 hash of the given input, returning as a 256-bit hash.
inline h256 sha3(bytesConstRef _input) noexcept
{
  h256 ret;
  sha3(_input, ret.ref());
  return ret;
}

inline SecureFixedHash<32> sha3Secure(bytesConstRef _input) noexcept
{
  SecureFixedHash<32> ret;
  sha3(_input, ret.writable().ref());
  return ret;
}

/// Calculate SHA3-256 hash of the given input, returning as a 256-bit hash.
inline h256 sha3(bytes const& _input) noexcept
{
  return sha3(bytesConstRef(&_input));
}

inline SecureFixedHash<32> sha3Secure(bytes const& _input) noexcept
{
  return sha3Secure(bytesConstRef(&_input));
}

/// Calculate SHA3-256 hash of the given input (presented as a binary-filled string), returning as a 256-bit hash.
inline h256 sha3(std::string const& _input) noexcept
{
  return sha3(bytesConstRef(_input));
}

inline SecureFixedHash<32> sha3Secure(std::string const& _input) noexcept
{
  return sha3Secure(bytesConstRef(_input));
}

/// Calculate SHA3-256 hash of the given input (presented as a FixedHash), returns a 256-bit hash.
template <unsigned N>
inline h256 sha3(FixedHash<N> const& _input) noexcept
{
  return sha3(_input.ref());
}

template <unsigned N>
inline SecureFixedHash<32> sha3Secure(FixedHash<N> const& _input) noexcept
{
  return sha3Secure(_input.ref());
}

/// Fully secure variants are equivalent for sha3 and sha3Secure.
inline SecureFixedHash<32> sha3(bytesSec const& _input) noexcept
{
  return sha3Secure(_input.ref());
}

inline SecureFixedHash<32> sha3Secure(bytesSec const& _input) noexcept
{
  return sha3Secure(_input.ref());
}

template <unsigned N>
inline SecureFixedHash<32> sha3(SecureFixedHash<N> const& _input) noexcept
{
  return sha3Secure(_input.ref());
}

template <unsigned N>
inline SecureFixedHash<32> sha3Secure(SecureFixedHash<N> const& _input) noexcept
{
  return sha3Secure(_input.ref());
}

template<typename T>
h256 sha3( const T& t ) noexcept
{
  sha3_256_encoder e;
  fc_light::raw::pack(e,t);
  return e.result();
}

}

namespace fc
{

class variant;
void to_variant( const dev::h256& bi, variant& v );
void from_variant( const variant& v, dev::h256& bi );

}
