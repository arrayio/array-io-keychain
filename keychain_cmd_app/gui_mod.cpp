//
// Created by roman on 5/14/18.
//

#include "gui_mod.hpp"

using namespace keychain_app;

gui_mod_dummy::gui_mod_dummy()
{}

gui_mod_dummy::~gui_mod_dummy()
{}

dev::Public gui_mod_dummy::select_key() const
{
  auto& keyfiles = keyfile_singleton::instance();
  auto it = keyfiles.begin();
  if ( it==keyfiles.end() )
    return dev::Public();
  return it->second.keyinfo.public_key;
}
