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
    auto a = get_path();
    
    [ApplicationShared sharedInstance];
    
    NSRect frame = NSMakeRect(0, 0, 575, 361);
    MyDialog *dialog = [[MyDialog alloc] initWithFrame:frame];
    dialog.jsonString = [NSString stringWithUTF8String:raw_trx.c_str()];
    dialog.isSignTransaction = true;
    dialog.currentPath = [NSString stringWithUTF8String:a.parent_path().c_str()];
    [dialog runModal];
    
    std::string str = std::string([[[PassSyncStore sharedInstance] pass] UTF8String]);
    [[PassSyncStore sharedInstance] setPass:@""];
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}

byte_seq_t sec_mod_mac::get_passwd_on_create() const
{
    auto a = get_path();
    
    [ApplicationShared sharedInstance];
    NSRect frame = NSMakeRect(0, 0, 575, 261);
    MyDialog *dialog = [[MyDialog alloc] initWithFrame:frame];
    dialog.isSignTransaction = false;
    dialog.currentPath = [NSString stringWithUTF8String:a.parent_path().c_str()];
    [dialog runModal];

    std::string str = std::string([[[PassSyncStore sharedInstance] pass] UTF8String]);
    [[PassSyncStore sharedInstance] setPass:@""];
    keychain_app::byte_seq_t pass(str.begin(), str.end());
    return pass;
}

bfs::path get_path() {
    unsigned int bufferSize = 512;
    std::vector<char> buffer(bufferSize + 1);
    if(_NSGetExecutablePath(&buffer[0], &bufferSize))
    {
        buffer.resize(bufferSize);
        _NSGetExecutablePath(&buffer[0], &bufferSize);
    }
    bfs::path path = &buffer[0];
    return path;
}
