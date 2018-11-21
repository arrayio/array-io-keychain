//
// Created by roman on 5/14/18.
//

#include "keychain_wrapper.hpp"

using namespace keychain_app;

std::string keychain_app::keychain_wrapper(const secure_dlg_mod_base* secure_dlg, const fc_light::variant& var)
{
  auto& keychain_= keychain::instance(secure_dlg);
  return keychain_(var);
}