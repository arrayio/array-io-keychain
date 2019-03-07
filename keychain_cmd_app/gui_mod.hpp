//
// Created by roman on 16/1/19.
//

#pragma once

#include <keychain_lib/keychain.hpp>

namespace keychain_app
{

class gui_mod_dummy: public gui_mod_base
{
public:
  gui_mod_dummy();
  virtual ~gui_mod_dummy();
  virtual dev::Public select_key() const override;
  virtual dev::bytes entropy() const override;
};

}
