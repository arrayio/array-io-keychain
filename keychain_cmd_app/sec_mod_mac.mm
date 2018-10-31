//
//  sec_mod_mac.cpp
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//

#include "sec_mod_mac.hpp"
#import <Foundation/Foundation.h>
#import "MyDialog.h"
#import "PassSyncStore.h"

using namespace keychain_app;

sec_mod_mac::sec_mod_mac()
{}

sec_mod_mac::~sec_mod_mac()
{}

std::string sec_mod_mac::get_uid() const
{
    return std::string("uid");
}

void sec_mod_mac::print_mnemonic(const string_list& mnemonic) const
{
}

byte_seq_t sec_mod_mac::get_passwd_trx_raw(const std::string& raw_trx) const
{
    [NSApplication sharedApplication];
//    ProcessSerialNumber psn;
//    GetCurrentProcess( &psn );
//    TransformProcessType(&psn,kProcessTransformToForegroundApplication);
//    SetFrontProcess( &psn );
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [NSApp activateIgnoringOtherApps:YES];
    
    NSRect frame = NSMakeRect(0, 0, 200, 200);
    MyDialog *dialog = [[MyDialog alloc] initWithFrame:frame];
    [dialog runModal];
    std::string str = std::string([[[PassSyncStore sharedInstance] pass] UTF8String]);
    [[PassSyncStore sharedInstance] setPass:@""];
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}

byte_seq_t sec_mod_mac::get_passwd_on_create() const
{
    std::string str = "blank";
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}
