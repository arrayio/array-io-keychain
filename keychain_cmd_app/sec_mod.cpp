//
// Created by roman on 5/14/18.
//

#include "sec_mod.hpp"

using namespace keychain_app;

sec_mod_dummy::sec_mod_dummy()
{}

sec_mod_dummy::~sec_mod_dummy()
{}

void sec_mod_dummy::print_mnemonic(const string_list& mnemonic) const
{
}

byte_seq_t sec_mod_dummy::get_passwd_trx_raw(const std::string& raw_trx) const
{
  std::string str = "blank";
  keychain_app::byte_seq_t pass(str.begin(), str.end());
  return pass;
}

byte_seq_t sec_mod_dummy::get_passwd_on_create() const
{
  std::string str = "blank";
  keychain_app::byte_seq_t pass(str.begin(), str.end());
  return pass;
}

