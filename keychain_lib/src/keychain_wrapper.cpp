//
// Created by roman on 5/14/18.
//

#include "keychain_wrapper.hpp"

using namespace keychain_app;

void keychain_app::keychain_wrapper(const secure_dlg_mod_base* secure_dlg, const fc::variant& var)
{
  keychain keychain_(secure_dlg->get_uid());
  keychain_.get_passwd_trx_raw.connect(std::bind(&secure_dlg_mod_base::get_passwd_trx_raw, secure_dlg, std::placeholders::_1));
  keychain_.get_passwd_trx.connect(std::bind(&secure_dlg_mod_base::get_passwd_trx, secure_dlg, std::placeholders::_1));
  keychain_.get_passwd.connect(std::bind(&secure_dlg_mod_base::get_passwd, secure_dlg, std::placeholders::_1));
  keychain_.print_mnemonic.connect(std::bind(&secure_dlg_mod_base::print_mnemonic, secure_dlg, std::placeholders::_1));
  
  keychain_(var);
}