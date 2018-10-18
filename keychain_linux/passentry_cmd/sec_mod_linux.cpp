//
// Created by user on 18.10.18.
//

#include "sec_mod_linux.hpp"

using namespace keychain_app;

#include <pass_entry_term.hpp>

sec_mod_linux::sec_mod_linux()
{}

sec_mod_linux::~sec_mod_linux()
{}

std::string sec_mod_linux::get_uid() const
{
    return std::string("uid");
}

void sec_mod_linux::print_mnemonic(const string_list& mnemonic) const
{
}

byte_seq_t sec_mod_linux::get_passwd_trx_raw(const std::string& raw_trx) const
{
    auto pass_entry = pass_entry_term();
    auto map_instance = map_translate_singletone::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, raw_trx);
    return pass;
}

byte_seq_t sec_mod_linux::get_passwd_on_create() const
{
    auto pass_entry = pass_entry_term();
    auto map_instance = map_translate_singletone::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, "");
    return pass;
}
