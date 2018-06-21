//
// Created by roman on 5/14/18.
//

#include "secure_module_dummy.hpp"

using namespace keychain_app;

sec_mod_dummy::sec_mod_dummy()
{}

sec_mod_dummy::~sec_mod_dummy()
{}

std::wstring sec_mod_dummy::get_passwd_trx(const graphene::chain::transaction& trx) const
{
  /*
  auto pass_entry = pass_entry_term();
  auto map_instance = map_translate_singletone::instance(pass_entry._display);
  auto pass = pass_entry.fork_gui(map_instance.map, trx);
  std::wcout <<"password: "<< pass << std::endl;
  return pass;
  */
  return std::wstring(L"empty");
}

std::string sec_mod_dummy::get_uid() const
{
  return std::string("uid");
}

void sec_mod_dummy::print_mnemonic(const string_list& mnemonic) const
{
}

std::wstring sec_mod_dummy::get_passwd_trx_raw(const std::string& raw_trx) const
{
  auto pass_entry = pass_entry_term();
  auto map_instance = map_translate_singletone::instance(pass_entry._display);
  auto pass = pass_entry.fork_gui(map_instance.map, raw_trx);
  std::wcout <<"password: "<< pass << std::endl;
  return pass;
}

std::wstring sec_mod_dummy::get_passwd(const std::string& str) const
{
/*
  auto pass_entry = pass_entry_term();
  auto map_instance = map_translate_singletone::instance(pass_entry._display);
  auto pass = pass_entry.fork_gui(map_instance.map);
  std::wcout <<"password: "<< pass << std::endl;
  return pass;
  */
  return std::wstring(L"empty");
}