//
// Created by user on 18.10.18.
//

#include "sec_mod_linux.hpp"
#include <keychain_lib/secmod_parser_cmd.hpp>


using namespace keychain_app;

#include <pass_entry_term.hpp>

namespace sm_cmd = keychain_app::secmod_commands;


std::string sec_mod_linux::exec_cmd(const std::string& json_cmd) const
{
    auto& log = logger_singleton::instance();
    BOOST_LOG_SEV(log.lg, info) << "Send to sec_mod_linux: " + json_cmd;

    sm_cmd::secmod_parser_f parser;
    auto etype = parser(json_cmd);

    keychain_app::byte_seq_t result_pass;
    result_pass.reserve(512);

    auto pass_entry = pass_entry_term();
    pass_entry.confirm = (etype==sm_cmd::events_te::create_key) ? true: false;
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    result_pass = pass_entry.fork_gui(map_instance.map, json_cmd);

    std::string result;
    sm_cmd::secmod_response_common response;
    if (result_pass.empty())
    {
        response.etype = sm_cmd::response_te::null;
        result = fc_light::json::to_pretty_string(response);
    }
    else
    {
        response.etype = sm_cmd::response_te::password;
        response.params = result_pass;
        result = fc_light::json::to_pretty_string(response);
    }
    return result;
}

sec_mod_linux::sec_mod_linux()
{}

sec_mod_linux::~sec_mod_linux()
{}
/*

void sec_mod_linux::print_mnemonic(const string_list& mnemonic) const
{
}

byte_seq_t sec_mod_linux::get_passwd_trx(const std::string& raw_trx) const
{
    auto a = master::cmd<master::cmds::rawtrx>(raw_trx);
    auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

    auto pass_entry = pass_entry_term(false);
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, mes);
    return pass;
}

byte_seq_t sec_mod_linux::get_passwd_unlock(const std::string& keyname, int unlock_time) const
{
    auto a = master::cmd<master::cmds::unlock>(keyname, unlock_time);
    auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

    auto pass_entry = pass_entry_term(false);
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, mes);
    return pass;
}

byte_seq_t sec_mod_linux::get_passwd_on_create(const std::string& keyname) const
{
    auto a = master::cmd<master::cmds::create>(keyname);
    auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

    auto pass_entry = pass_entry_term(true);
    auto map_instance = map_translate_singleton::instance(pass_entry._display);
    auto pass = pass_entry.fork_gui(map_instance.map, mes);
    return pass;
}
*/