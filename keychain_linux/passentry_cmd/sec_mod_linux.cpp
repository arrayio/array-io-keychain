//
// Created by user on 18.10.18.
//

#include "sec_mod_linux.hpp"

using namespace keychain_app;

#include <pass_entry_term.hpp>
#include <cmd.hpp>

sec_mod_linux::sec_mod_linux()
{}

sec_mod_linux::~sec_mod_linux()
{}


void sec_mod_linux::print_mnemonic(const string_list& mnemonic) const
{
}

byte_seq_t sec_mod_linux::get_passwd_trx(const std::string& raw_trx) const
{
    auto a = master::cmd<master::cmds::rawtrx>(raw_trx);
    auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

    auto pass_entry = pass_entry_term();
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, mes);
    return pass;
}

byte_seq_t sec_mod_linux::get_passwd_unlock(const std::string& keyname, int unlock_time) const
{
    auto a = master::cmd<master::cmds::unlock>(keyname, unlock_time);
    auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

    auto pass_entry = pass_entry_term();
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, mes);
    return pass;
}

byte_seq_t sec_mod_linux::get_passwd_on_create(const std::string& keyname) const
{
    auto a = master::cmd<master::cmds::create>(keyname);
    auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

    auto pass_entry = pass_entry_term();
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, mes);
    return pass;
}
