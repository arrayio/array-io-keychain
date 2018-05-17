//
// Created by roman on 5/14/18.
//

#ifndef KEYCHAINAPP_SECURE_MODULE_DUMMY_HPP
#define KEYCHAINAPP_SECURE_MODULE_DUMMY_HPP

#include <keychain_lib/keychain_wrapper.hpp>

namespace keychain_app
{

class sec_mod_dummy: public secure_dlg_mod_base
{
public:
  sec_mod_dummy();
  virtual ~sec_mod_dummy();
  virtual std::string get_passwd_trx_raw(const std::string& raw_trx) const override;
  virtual std::string get_passwd_trx(const graphene::chain::transaction& trx) const override;
  virtual std::string get_passwd(const std::string& str) const override;
  virtual void print_mnemonic(const string_list& mnemonic) const override;
  virtual std::string get_uid() const override;
  
  
};

}

#endif //KEYCHAINAPP_SECURE_MODULE_DUMMY_HPP
