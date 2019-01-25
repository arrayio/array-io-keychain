#pragma once

#include <keychain_lib/keychain.hpp>
#include "SecurityManager.h"


class SecureModuleWrapper : public keychain_app::secure_dlg_mod_base
{
public:
  virtual ~SecureModuleWrapper();
  virtual std::string exec_cmd(const std::string& json_cmd) const override;
};
