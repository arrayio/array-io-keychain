//
// Created by roman on 4/24/18.
//

#ifndef KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP
#define KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP

#include <string>
#include "keychain.hpp"

namespace keychain_app {

class secure_dlg_mod_base
{
public:
  using string_list = std::list<std::wstring>;
  
  virtual ~secure_dlg_mod_base(){}
  virtual std::wstring get_passwd_trx_raw(const std::string& raw_trx) const = 0;
  virtual std::wstring get_passwd_trx(const graphene::chain::transaction& trx) const = 0;
  virtual std::wstring get_passwd(const std::string& str) const = 0;
  virtual void print_mnemonic(const string_list& mnemonic) const = 0;
  virtual std::string get_uid() const = 0;
};


void keychain_wrapper(const secure_dlg_mod_base* secure_dlg, const fc::variant& var);

}

#endif //KEYCHAINAPP_KEYCHAIN_WRAPPER_HPP
