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
  return std::wstring(L"blank_password");
  auto map_instance = map_translate_singletone::instance(pass_entry._display);
  auto pass = pass_entry.fork_gui(map_instance.map, raw_trx);
  std::wcout <<"password: "<< pass << std::endl;
  return pass;
}

byte_seq_t sec_mod_dummy::get_passwd_on_create() const
{
/*
  auto pass_entry = pass_entry_term();
  auto map_instance = map_translate_singletone::instance(pass_entry._display);
  auto pass = pass_entry.fork_gui(map_instance.map);
  std::wcout <<"password: "<< pass << std::endl;
  return pass;
  */
  return std::wstring(L"blank_password");
}