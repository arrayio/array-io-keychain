//
// Created by user on 18.10.18.
//

#include "sec_mod_linux.hpp"
#include <keychain_lib/secmod_parser_cmd.hpp>


using namespace keychain_app;

#include <pass_entry_term.hpp>
#include <cmd.hpp>

namespace sm_cmd = keychain_app::secmod_commands;

std::string keychain_app::sec_mod_dummy::exec_cmd(const std::string& json_cmd) const
{
    secmod_commands::secmod_parser_f parser;
    auto etype = parser(json_cmd);
    int unlock_time = 0;
    switch (etype)
    {
        case secmod_commands::events_te::create_key:
        case secmod_commands::events_te::sign_hex:
        case secmod_commands::events_te::sign_hash:
        case secmod_commands::events_te::remove_key:
        case secmod_commands::events_te::export_keys:
        case secmod_commands::events_te::import_keys:
        case secmod_commands::events_te::unlock:
        {
            std::string str = "blank";
            keychain_app::byte_seq_t pass(str.begin(), str.end());

            secmod_commands::secmod_resonse_common response;
            response.etype = secmod_commands::response_te::boolean;
//    response.etype = secmod_commands::response_te::password;
//    response.params = pass;
            response.params = true;
            return fc_light::json::to_pretty_string(response);
        }
        default:
        {
            FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", etype));
        }
    }


}


std::string sec_mod_linux::exec_cmd(const std::string& json_cmd) const
{
    auto& log = logger_singleton::instance();
    BOOST_LOG_SEV(log.lg, info) << "Send to sec_mod_linux: " + json_cmd;

    sm_cmd::secmod_parser_f parser;
    auto etype = parser(json_cmd);
    int unlock_time = 0;
    switch (etype)
    {
        case sm_cmd::events_te::create_key:
        {
            auto cmd = parser.params<sm_cmd::events_te::create_key>();

            auto a = master::cmd<master::cmds::create>(fc_light::json::to_string(cmd.keyname ));
            auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

            auto pass_entry = pass_entry_term(true);
            auto map_instance = map_translate_singleton::instance(pass_entry._display);
            auto pass = pass_entry.fork_gui(map_instance.map, mes);
            return pass;
        }
        case sm_cmd::events_te::sign_hex:
        {
            auto cmd = parser.params<sm_cmd::events_te::sign_hex>();
            unlock_time = cmd.unlock_time;

            auto a = master::cmd<master::cmds::rawtrx>(fc_light::json::to_string(cmd.trx_view) );
            auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

            auto pass_entry = pass_entry_term(false);
            auto map_instance = map_translate_singleton::instance(pass_entry._display);
            auto pass = pass_entry.fork_gui(map_instance.map, mes);
            break;
            return pass;
        }
        case sm_cmd::events_te::sign_hash:
        case sm_cmd::events_te::remove_key:
        case sm_cmd::events_te::export_keys:
        case sm_cmd::events_te::import_keys:
        case sm_cmd::events_te::unlock:
        {
            auto cmd = parser.params<sm_cmd::events_te::unlock>();

            auto a = master::cmd<master::cmds::unlock>(cmd.keyname, cmd.unlock_time);
            auto mes = fc_light::json::to_string(fc_light::variant(static_cast<const master::cmd_base&>(a)));

            auto pass_entry = pass_entry_term(false);
            auto map_instance = map_translate_singleton::instance(pass_entry._display);
            auto pass = pass_entry.fork_gui(map_instance.map, mes);
            return pass;

            break;
        }
    }

    keychain_app::byte_seq_t result_pass;
    result_pass.reserve(512);

    std::string result;
    result_pass = { 'b', 'l', 'a', 'n', 'k' };
    sm_cmd::secmod_resonse_common response;
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