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
  return std::wstring(L"blank_password");
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
 return std::wstring(L"blank_password");
}

std::wstring sec_mod_dummy::get_passwd(const std::string& str) const
{
  uid_t ruid, euid, suid;
  uid_t oruid, oeuid, osuid; // original

  if (getresuid(&oruid, &oeuid, &osuid) == -1 )throw std::runtime_error("terminal: getresuid()");
  std::cout<<"terminal - ruid: "<< oruid <<" euid: "<<oeuid<<" suid: "<<osuid<< std::endl;
  if (setresuid(oruid, oruid, osuid)    == -1 )throw std::runtime_error("terminal: setresuid()");
  if (getresuid(&ruid, &euid, &suid)    == -1 )throw std::runtime_error("terminal: getresuid()");
  std::cout<<"terminal - ruid: "<< ruid <<" euid: "<<euid<<" suid: "<<suid<< std::endl;

  auto pass_entry = pass_entry_term();
  auto map_instance = map_translate_singletone::instance(pass_entry._display);
  auto pass = pass_entry.fork_gui(map_instance.map);

  if (setresuid(oruid, oeuid, osuid)    == -1 )throw std::runtime_error("terminal: setresuid()");
  if (getresuid(&ruid, &euid, &suid)    == -1 )throw std::runtime_error("terminal: getresuid()");
  std::cout<<"terminal - ruid: "<< ruid <<" euid: "<<euid<<" suid: "<<suid<< std::endl;

  std::wcout <<"password: "<< pass << std::endl;
  return std::wstring(L"blank_password");
}