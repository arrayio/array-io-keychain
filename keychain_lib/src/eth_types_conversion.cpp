#include <vector>
#include <algorithm>

#include <eth-crypto/core/FixedHash.h>
#include <fc_light/exception/exception.hpp>

#include "eth_types_conversion.hpp"

namespace fc_light
{

template <unsigned N>
void to_variant_eth_impl( const dev::FixedHash<N>& hash, fc_light::variant& v )
{
  std::vector<char> key_vec(hash.size);
  std::copy(hash.begin(), hash.end(), key_vec.begin());
  
  v = key_vec;
}

template <unsigned N>
void from_variant_eth_impl( const fc_light::variant& v, dev::FixedHash<N>& hash )
{
  using hash_t = typename std::remove_reference<decltype(hash)>::type;
  auto hex_str = v.as<std::string>();
  hash = hash_t(hex_str);
}

template <unsigned N>
void to_variant_eth_impl( const dev::SecureFixedHash<N>& hash, fc_light::variant& v )
{
  std::vector<char> key_vec(hash.size);
  auto sec_ref = hash.ref();
  std::copy(sec_ref.begin(), sec_ref.end(), key_vec.begin());
  
  v = key_vec;
}

template <unsigned N>
void from_variant_eth_impl( const fc_light::variant& v, dev::SecureFixedHash<N>& hash )
{
  auto ser = v.as<std::vector<uint8_t> >();
  FC_LIGHT_ASSERT(hash.size == ser.size());
  using hash_t = typename std::remove_reference<decltype(hash)>::type;
  hash=hash_t(ser);
}

void to_variant( const dev::Address& addr, variant&v )
{
  to_variant_eth_impl(addr,v);
}

void from_variant( const variant& v, dev::Address& addr )
{
  from_variant_eth_impl(v, addr);
}

void to_variant( const dev::Public& pk, variant& v )
{
  to_variant_eth_impl(pk, v);
}

void from_variant( const variant& v, dev::Public& pk )
{
  from_variant_eth_impl(v, pk);
}

void to_variant( const dev::Secret& key, variant& v )
{
  to_variant_eth_impl(key, v);
}

void from_variant( const variant& v, dev::Secret& key )
{
  from_variant_eth_impl(v, key);
}

void to_variant( const dev::Signature& sig, variant& v )
{
  to_variant_eth_impl(sig, v);
}

void from_variant( const variant& v, dev::Signature& sig )
{
  from_variant_eth_impl(v, sig);
}

void to_variant( const dev::h256& h, variant& v )
{
  to_variant_eth_impl(h, v);
}

void from_variant( const variant& v, dev::h256& h )
{
  from_variant_eth_impl(v, h);
}

}