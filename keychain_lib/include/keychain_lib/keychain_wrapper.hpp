//
// Created by roman on 4/24/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP
#define KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP

#include <string>
#include <vector>
#include "keychain.hpp"

namespace keychain_app {

using byte_seq_t = std::vector<char>;

std::string keychain_wrapper(const secure_dlg_mod_base* secure_dlg, const fc_light::variant& var);

}

#endif //KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP
