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

using namespace keychain_app;

sec_mod_mac::sec_mod_mac()
{}

sec_mod_mac::~sec_mod_mac()
{}

bfs::path get_path();


void sec_mod_mac::print_mnemonic(const string_list& mnemonic) const
{
}

byte_seq_t sec_mod_mac::get_passwd_trx_raw(const std::string& raw_trx) const
{
    [ApplicationShared sharedInstance];
    NSError *error;
    ResponseModel *model = [[ResponseModel alloc] initWithString:[NSString stringWithUTF8String:raw_trx.c_str()] error:&error];
    NSLog(@"Error %@", error);
    
    NSRect frame = NSMakeRect(0, 0, (error == nil && model.swap != NULL) ? 825 : 575, (error == nil && model.swap != NULL) ? 521 : 361);
    DialogWC *dialog = [[DialogWC alloc] initWithFrame:frame];
    dialog.jsonString = [NSString stringWithUTF8String:raw_trx.c_str()];
    dialog.jsonModel = model;
    dialog.isRawTransaction = (error == nil) ? false : true;
    dialog.isSignTransaction = true;
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

