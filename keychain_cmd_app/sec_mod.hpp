//
// Created by roman on 5/14/18.
//

#ifndef KEYCHAINAPP_SEC_MOD_HPP
#define KEYCHAINAPP_SEC_MOD_HPP

#include <keychain_lib/keychain.hpp>

namespace keychain_app
{

class sec_mod_dummy: public secure_dlg_mod_base
{
public:
  sec_mod_dummy();
  virtual ~sec_mod_dummy();
  virtual std::string exec_cmd(const std::string& json_cmd) const override;
private:
  static constexpr const char* pass_str = "blank_password";
};

}

#endif //KEYCHAINAPP_SEC_MOD_HPP
