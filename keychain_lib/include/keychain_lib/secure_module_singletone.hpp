//
// Created by roman on 5/14/18.
//

#ifndef KEYCHAINAPP_SECURE_MODULE_SINGLETONE_HPP
#define KEYCHAINAPP_SECURE_MODULE_SINGLETONE_HPP

#include <type_traits>

namespace keychain_app {

template<typename sec_mod_impl>
class secure_module
{
  static_assert(std::is_base_of<secure_dlg_mod_base, sec_mod_impl>::value,"");
public:
  static const secure_dlg_mod_base* instance()
  {
    static const sec_mod_impl sec_mod;
    return &sec_mod;
  }
};

}

#endif //KEYCHAINAPP_SECURE_MODULE_SINGLETONE_HPP
