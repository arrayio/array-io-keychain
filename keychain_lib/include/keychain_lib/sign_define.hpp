//
// Created by roman on 4/6/18.
//

#ifndef KEYCHAINAPP_SIGN_DEFINE_HPP
#define KEYCHAINAPP_SIGN_DEFINE_HPP

#include <boost/variant.hpp>

namespace keychain_app
{

using unit_t =  std::vector<char> ;
using unit_list_t = std::vector< unit_t >;
static const char* KEY_DEFAULT_PATH = "./key_data";

}

#endif //KEYCHAINAPP_SIGN_DEFINE_HPP
