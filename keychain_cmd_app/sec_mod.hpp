//
// Created by roman on 5/14/18.
//

#ifndef KEYCHAINAPP_SEC_MOD_HPP
#define KEYCHAINAPP_SEC_MOD_HPP

#include <keychain_lib/keychain_wrapper.hpp>

namespace keychain_app
{

class sec_mod_dummy: public secure_dlg_mod_base
{
public:
  sec_mod_dummy();
  virtual ~sec_mod_dummy();
  virtual keychain_app::byte_seq_t  get_passwd_trx_raw(const std::string& raw_trx) const override;
  virtual keychain_app::byte_seq_t get_passwd_on_create() const override;
  virtual void print_mnemonic(const string_list& mnemonic) const override;
private:
  static constexpr const char* pass_str = "blank_password";
};

}

#endif //KEYCHAINAPP_SEC_MOD_HPP
