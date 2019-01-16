//
// Created by roman on 5/14/18.
//
#pragma once

#include <type_traits>

namespace keychain_app {

template <typename mod_base>
class module_singleton
{
public:
  
  template <typename mod_impl>
  static const mod_base* instance()
  {
    static_assert(std::is_base_of<mod_base, mod_impl>::value,"mod_base is not base class for mod_impl");
    static const mod_impl mod;
    return &mod;
  }
};

}

