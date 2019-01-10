#pragma once

#include <keychain_lib/keychain_wrapper.hpp>
#include "SecurityManager.h"


class SecureModuleWrapper : public keychain_app::secure_dlg_mod_base
{
public:
  virtual ~SecureModuleWrapper();
  virtual keychain_app::byte_seq_t get_passwd_trx(const std::string& json_cmd) const override;
  virtual keychain_app::byte_seq_t get_passwd_unlock(const std::string& keyname, int unlock_time) const override;
  virtual keychain_app::byte_seq_t get_passwd_on_create(const std::string& keyname = std::string("")) const override;
  virtual void print_mnemonic(const string_list& mnemonic) const override;
private:
  keychain_app::byte_seq_t _startSecureDesktop(const std::string& str, int unlock_time = 0) const;
};
