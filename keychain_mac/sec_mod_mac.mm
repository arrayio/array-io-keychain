//
//  sec_mod_mac.cpp
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//

#include "sec_mod_mac.hpp"
#import <Foundation/Foundation.h>
#import "DialogWC.h"
#import "PassSyncStore.h"
#import "ApplicationShared.h"

#include <vector>
#include <boost/filesystem.hpp>
#include <mach-o/dyld.h>
#include <keychain_lib/secmod_parser_cmd.hpp>
#include <keychain_lib/keychain_logger.hpp>

using namespace keychain_app;

sec_mod_mac::sec_mod_mac()
{}

sec_mod_mac::~sec_mod_mac()
{}

bfs::path get_path();

std::string keychain_app::sec_mod_mac::exec_cmd(const std::string& json_cmd) const
{
    auto& log = logger_singleton::instance();
    [ApplicationShared sharedInstance];
    NSRect frame;
    BOOST_LOG_SEV(log.lg, info) << "Send to secmod mac:" + json_cmd;
    secmod_commands::secmod_parser_f parser;
    auto etype = parser(json_cmd);
    int unlock_time = 0;
    bool is_json = true;
    bool no_password = false;
    std::string blockhain_type;
    switch (etype)
    {
        case secmod_commands::events_te::sign_trx: {
            NSLog(@"sign_hex");
            auto cmd = parser.params<secmod_commands::events_te::sign_trx>();
            is_json = cmd.is_parsed;
            switch (cmd.blockchain) {
                case secmod_commands::blockchain_secmod_te::unknown: {
                    NSLog(@"unknown");
                    frame = NSMakeRect(0, 0, 575, 245);
                    blockhain_type = "unknow";
                    break;
                }
                case secmod_commands::blockchain_secmod_te::ethereum: {
                    NSLog(@"ethereum");
                    frame = NSMakeRect(0, 0, 575, 361);
                    blockhain_type = "ethereum";
                    break;
                }
                case secmod_commands::blockchain_secmod_te::ethereum_swap: {
                    NSLog(@"ethereum_swap");
                    blockhain_type = "ethereum";
                    frame = NSMakeRect(0, 0, 825, 521);
                    break;
                }
                case secmod_commands::blockchain_secmod_te::bitcoin: {
                    NSLog(@"bitcoin_sec");
                    frame = NSMakeRect(0, 0, 575, is_json ? 440 : 240);
                    blockhain_type = "bitcoin";
                    break;
                }
            }
            
            no_password = cmd.no_password;
            
            DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
            dialog.jsonString = [NSString stringWithUTF8String:json_cmd.c_str()];
            dialog.isJson = is_json;
            dialog.unlockTime = unlock_time;
            dialog.unlockOnly = false;
            dialog.isSignTransaction = true;
            dialog.isRawhash = false;
            dialog.etype = etype;
            dialog.keyname = [NSString stringWithUTF8String:cmd.keyname.c_str()];
            dialog.parser = parser;
            dialog.isPasswordRequire = !cmd.no_password;
            dialog.expertModeString = [NSString stringWithUTF8String:keychain_app::secmod_commands::to_expert_mode_string(cmd).c_str()];
            dialog.blockhainType = [NSString stringWithUTF8String:blockhain_type.c_str()];
            [dialog runModal];
            break;
        }
        case secmod_commands::events_te::sign_hash: {
            auto cmd = parser.params<secmod_commands::events_te::sign_hash>();
            no_password = cmd.no_password;
            frame = NSMakeRect(0, 0, 1024, 285);
            blockhain_type = "hash";
            DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
            dialog.jsonString = [NSString stringWithUTF8String:json_cmd.c_str()];
            dialog.isJson = false;
            dialog.unlockTime = unlock_time;
            dialog.unlockOnly = false;
            dialog.isSignTransaction = true;
            dialog.keyname = [NSString stringWithUTF8String:cmd.keyname.c_str()];
            dialog.isRawhash = true;
            dialog.etype = etype;
            dialog.parser = parser;
            dialog.expertModeString = [NSString stringWithUTF8String:cmd.hash.c_str()];
            dialog.blockhainType = [NSString stringWithUTF8String:blockhain_type.c_str()];
            dialog.isPasswordRequire = !cmd.no_password;
            [dialog runModal];
            break;
        }
        case secmod_commands::events_te::unlock:
        {
            NSLog(@"unlock");
            auto cmd = parser.params<secmod_commands::events_te::unlock>();
            no_password = cmd.no_password;
            unlock_time = cmd.unlock_time;
            frame = NSMakeRect(0, 0, 575, 210);
        
            DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
            dialog.isSignTransaction = true;
            dialog.unlockTime = unlock_time;
            dialog.keyname = [NSString stringWithUTF8String:cmd.keyname.c_str()];
            dialog.unlockOnly = true;
            dialog.isRawhash = false;
            dialog.parser = parser;
            dialog.isPasswordRequire = !cmd.no_password;
            dialog.etype = etype;
            [dialog runModal];
            break;
        }
        default:
        {
            FC_LIGHT_THROW_EXCEPTION(fc_light::internal_error_exception, "Secmod command is not implemented, etype = %{ETYPE}", ("ETYPE", etype));
        }
    }

    secmod_commands::secmod_response_common response;
    if ([[PassSyncStore sharedInstance] buttonClickType] == ButtonClickTypeCancel) {
        response.etype = secmod_commands::response_te::canceled;
    } else if ([[PassSyncStore sharedInstance] buttonClickType] == ButtonClickTypeOK) {
        if (no_password) {
            response.etype = secmod_commands::response_te::boolean;
            response.params = true;
        } else {
            std::string str = std::string([[[PassSyncStore sharedInstance] pass] UTF8String]);
            keychain_app::byte_seq_t pass(str.begin(), str.end());
            response.etype = secmod_commands::response_te::password;
            response.params = pass;
        }
    }
    
    [[PassSyncStore sharedInstance] setPass:@""];

    return fc_light::json::to_pretty_string(fc_light::variant(response));
}
