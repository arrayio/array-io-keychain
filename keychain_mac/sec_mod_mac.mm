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
using secmod_commands::secmod_parser_f;

sec_mod_mac::sec_mod_mac()
{}

sec_mod_mac::~sec_mod_mac()
{}

bfs::path get_path();


void sec_mod_mac::print_mnemonic(const string_list& mnemonic) const
{
}

keychain_app::byte_seq_t keychain_app::sec_mod_mac::get_passwd_unlock(const std::string& keyname, int unlock_time) const
{
    NSLog(@"get_passwd_unlock");
    [ApplicationShared sharedInstance];
    NSRect frame = NSMakeRect(0, 0, 575, 210);
    
    DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
    dialog.isSignTransaction = false;
    dialog.unlockTime = unlock_time;
    dialog.keyname = [NSString stringWithUTF8String:keyname.c_str()];
    dialog.unlockOnly = true;
    [dialog runModal];
    
    std::string str = std::string([[[PassSyncStore sharedInstance] pass] UTF8String]);
    [[PassSyncStore sharedInstance] setPass:@""];
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}


byte_seq_t sec_mod_mac::get_passwd_trx(const std::string& raw_trx) const
{
    auto log = logger_singletone::instance();
    NSLog(@"get_passwd_trx");
    [ApplicationShared sharedInstance];
    NSRect frame;
    
    secmod_parser_f cmd_parse;
    auto cmd_type = cmd_parse(raw_trx);
    
    auto unlock_time = cmd_parse.unlock_time(); //check unlock time. If unlock time > 0 print red lock icon with text warning.
    auto is_json = cmd_parse.is_json();//need to check parse success. If json is false > 0 print red lock icon with text warning.
    std::string blockhain_type;
    NSLog(@"cmd_type %d", cmd_type);
    switch (cmd_type)
    {
        case keychain_app::secmod_commands::blockchain_secmod_te::unknown:
        {
            frame = NSMakeRect(0, 0, 575, 245);
            blockhain_type = "unknow";
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::ethereum:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "ethereum";
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::bitcoin:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "bitcoin";
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::rawhash:
        {
            frame = NSMakeRect(0, 0, 1024, 285);
            blockhain_type = "hash";
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap:
        {
            blockhain_type = "ethereum";
            frame = NSMakeRect(0, 0, 825, 521);
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::parse_error:
        {
            NSLog(@"PARSE_ERROR. Code -1.");
            BOOST_LOG_SEV(log.lg, info) << "PARSE_ERROR. Code -1.";
            std::string str = std::string([[NSString new] UTF8String]);
            keychain_app::byte_seq_t pass(str.begin(), str.end());
            return pass;
        }
            break;
        default:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "";
        }
            break;
    }
    
    DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
    dialog.jsonString = [NSString stringWithUTF8String:raw_trx.c_str()];
    dialog.isJson = is_json;
    dialog.unlockTime = unlock_time;
    dialog.unlockOnly = false;
    dialog.isSignTransaction = true;
    dialog.keyname = [NSString stringWithUTF8String:cmd_parse.keyname().c_str()];
    dialog.expertModeString = [NSString stringWithUTF8String:cmd_parse.to_expert_mode_string().c_str()];
    dialog.blockhainType = [NSString stringWithUTF8String:blockhain_type.c_str()];
    [dialog runModal];
    
    std::string str = std::string([[[PassSyncStore sharedInstance] pass] UTF8String]);
    [[PassSyncStore sharedInstance] setPass:@""];
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}

byte_seq_t sec_mod_mac::get_passwd_on_create() const
{
    [ApplicationShared sharedInstance];
    NSRect frame = NSMakeRect(0, 0, 575, 300);
    DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
    dialog.isSignTransaction = false;
    [dialog runModal];

    std::string str = std::string([[[PassSyncStore sharedInstance] pass] UTF8String]);
    [[PassSyncStore sharedInstance] setPass:@""];
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}
