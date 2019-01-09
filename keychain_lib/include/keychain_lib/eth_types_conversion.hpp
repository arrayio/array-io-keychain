//
// Created by roman on 19/12/18.
//

#ifndef KEYCHAINAPP_ETH_TYPES_CONVERSION_HPP
#define KEYCHAINAPP_ETH_TYPES_CONVERSION_HPP


namespace dev
{
template <unsigned N>
class FixedHash;
template <unsigned N>
class SecureFixedHash;

using Address = FixedHash<20>;
using Signature = FixedHash<65>;
using Public = FixedHash<64>;
using h256 = FixedHash<32>;
using Secret = SecureFixedHash<32>;

}

namespace fc_light
{

void to_variant( const dev::Address& addr, variant&v );
void from_variant( const variant& v, dev::Address& addr );
void to_variant( const dev::Public& key, variant& v );
void from_variant( const variant& v, dev::Public& key );
void to_variant( const dev::Secret& key, variant& v );
void from_variant( const variant& v, dev::Secret& key );
void to_variant( const dev::Signature& sig, variant& v );
void from_variant( const variant& v, dev::Signature& sig );
void to_variant( const dev::h256& h, variant& v );
void from_variant( const variant& v, dev::h256& h );

} //fc_light

#endif //KEYCHAINAPP_ETH_TYPES_CONVERSION_HPP
