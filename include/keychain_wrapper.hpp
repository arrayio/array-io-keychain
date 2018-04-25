//
// Created by roman on 4/24/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP
#define KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP

#include <string>
#include "keychain.hpp"

namespace keychain_app
{

//NOTE: for debug only
struct get_password_dummy
{
    static std::string get(){
      return std::string("blank_password");
    }
};

using get_password_functor = std::function<std::string()>;

//TODO: put here platform specific implementation
template <typename get_password_f>
struct keychain_wrapper: get_password_f
{
    static void exec(const fc::variant& var)
    {
      get_password_functor f = std::bind(&get_password_f::get);
      keychain keychain_(std::move(f));
      keychain_(var);
    }
};

using keychain_dummy_wrapper = keychain_wrapper<get_password_dummy>;

}

#endif //KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP
