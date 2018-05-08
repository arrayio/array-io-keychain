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

struct get_uid_dummy {
  static std::string get()
  {
    return std::string("uid");
  }
};

using get_password_functor = std::function<std::string()>;
using get_uid_functor = std::function<std::string()>;

//TODO: put here platform specific implementation
template <typename get_password_f, typename get_uid_functor>
struct keychain_wrapper: get_password_f, get_uid_functor
{
    static void exec(const fc::variant& var)
    {
      //TODO: add static assert to compare get_password_f and get_uid_functor signature
      // with get_password_functor get_uid_functor
      // to prevent difficultly analyze complile errors
      get_password_functor f = std::bind(&get_password_f::get);
      keychain keychain_(std::move(f), get_uid_functor::get());
      keychain_(var);
    }
};

using keychain_dummy_wrapper = keychain_wrapper<get_password_dummy, get_uid_dummy>;

}

#endif //KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP
