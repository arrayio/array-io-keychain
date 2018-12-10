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
    std::string str = "blank";
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}


byte_seq_t sec_mod_mac::get_passwd_trx(const std::string& raw_trx) const
{
    [ApplicationShared sharedInstance];
    NSRect frame;
    
    secmod_parser_f cmd_parse;
    auto cmd_type = cmd_parse(raw_trx);
    
    auto unlock_time = cmd_parse.unlock_time(); //check unlock time. If unlock time > 0 print red lock icon with text warning.
    auto is_json = cmd_parse.is_json();//need to check parse success. If json is false > 0 print red lock icon with text warning.
    std::string blockhain_type;
    
    switch (cmd_type)
    {
        case keychain_app::secmod_commands::blockchain_secmod_te::unknown:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "";
            //auto trx_str = cmd_parse.to_raw_tx();//raw hex transaction
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::ethereum:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "ethereum";
            auto eth_trx = cmd_parse.to_ethereum();
            auto from_str = eth_trx.from;
            auto eth_data = eth_trx.trx_info;
            auto to_str = eth_data.to;
            auto amount = eth_data.value;
            auto expert_mode_str = cmd_parse.to_expert_mode_string();
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::bitcoin:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "bitcoin";
            auto bitcoin_trx = cmd_parse.to_bitcoin();
            auto from_str = bitcoin_trx.from;
            auto bitcoin_data = bitcoin_trx.trx_info;
            auto num_vouts = bitcoin_data.num_vouts;//chech num of vouts
            auto vout1 = bitcoin_data.vouts[0];
            auto to_str1 = vout1.address;
            auto amount1 = vout1.amount;
            //etc
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::rawhash:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "";
            auto trx_str = cmd_parse.to_raw_trx_string();//raw hex transaction
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap:
        {
            blockhain_type = "ethereum";
            frame = NSMakeRect(0, 0, 825, 521);
            auto swap_trx = cmd_parse.to_ethereum_swap();
            auto from_str = swap_trx.from;
            auto swap_info = swap_trx.swap_info;
            switch (swap_info.action)
            {
                    //TODO: need impleentation
            }
            auto eth_data = swap_trx.trx_info;
            auto to_str = eth_data.to;
            auto amount = eth_data.value;
            auto expert_mode_str = cmd_parse.to_expert_mode_string();
        }
            break;
        case keychain_app::secmod_commands::blockchain_secmod_te::parse_error:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "";
            //some error msg into log
        }
            break;
        default:
        {
            frame = NSMakeRect(0, 0, 575, 361);
            blockhain_type = "";
            //some error msg into log
        }
            break;
    }
    
//    NSError *error;
//    ResponseModel *model = [[ResponseModel alloc] initWithString:[NSString stringWithUTF8String:raw_trx.c_str()] error:&error];
//    NSLog(@"Error %@", error);
    
    DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
    dialog.jsonString = [NSString stringWithUTF8String:raw_trx.c_str()];
    dialog.isJson = is_json;
    dialog.isSignTransaction = true;
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
    NSRect frame = NSMakeRect(0, 0, 575, 261);
    DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
    dialog.isSignTransaction = false;
    [dialog runModal];

    std::string str = std::string([[[PassSyncStore sharedInstance] pass] UTF8String]);
    [[PassSyncStore sharedInstance] setPass:@""];
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}
