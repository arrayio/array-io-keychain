//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_SIGN_DEFINE_HPP
#define KEYCHAINAPP_SIGN_DEFINE_HPP

#include <boost/variant.hpp>
#include <fc_light/reflect/reflect.hpp>
#include <fc_light/reflect/variant.hpp>

namespace keychain_app
{

enum struct blockchain_te {
  unknown=0,
  array,
  bitshares,
  ethereum,
  bitcoin,
  rawhash
};

using unit_t =  std::vector<unsigned char> ;
using unit_list_t = std::vector< unit_t >;

}

FC_LIGHT_REFLECT_ENUM(keychain_app::blockchain_te, (unknown)(bitshares)(array)(ethereum)(bitcoin))

#endif //KEYCHAINAPP_SIGN_DEFINE_HPP
