//
// Created by roman on 5/14/18.
//

#include "secure_module_dummy.hpp"

using namespace keychain_app;

sec_mod_dummy::sec_mod_dummy()
{}

sec_mod_dummy::~sec_mod_dummy()
{}

std::string sec_mod_dummy::get_uid() const
{
  return std::string("uid");
}

void sec_mod_dummy::print_mnemonic(const string_list& mnemonic) const
{
}

byte_seq_t sec_mod_dummy::get_passwd_trx_raw(const std::string& raw_trx) const
{
  return byte_seq_t(pass_str, pass_str + strlen(pass_str));
}

byte_seq_t sec_mod_dummy::get_passwd_on_create() const
{
  return byte_seq_t(pass_str, pass_str + strlen(pass_str));
}