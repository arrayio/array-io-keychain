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


void sec_mod_linux::print_mnemonic(const string_list& mnemonic) const
{
}

byte_seq_t sec_mod_linux::get_passwd_trx(const std::string& raw_trx) const
{
    auto pass_entry = pass_entry_term();
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, raw_trx, master::cmds::rawtrx);
    return pass;
}

byte_seq_t sec_mod_linux::get_passwd_unlock(const std::string& keyname, int unlock_time) const
{
    auto pass_entry = pass_entry_term();
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, keyname  + std::string(", unlock_time: "+ std::to_string(unlock_time)) ,
            master::cmds::create);
    return pass;
}

byte_seq_t sec_mod_linux::get_passwd_on_create(const std::string keyname) const
{
    auto pass_entry = pass_entry_term();
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, "create", master::cmds::create);
    return pass;
}
