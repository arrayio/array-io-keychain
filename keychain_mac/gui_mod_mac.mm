//
//  gui_mod_mac.m
//  keychain
//
//  Created by Mikhail Lutskiy on 28/01/2019.
//

#import "gui_mod_mac.hpp"
#import "SelectKeyVC.h"
#import <Foundation/Foundation.h>
#import "ApplicationShared.h"
#import "PassSyncStore.h"
#import "EntropyCreatingVC.h"

using namespace keychain_app;

gui_mod_mac::gui_mod_mac()
{}

gui_mod_mac::~gui_mod_mac()
{}

dev::Public gui_mod_mac::select_key() const
{
    [ApplicationShared sharedInstance];
    auto& keyfiles = keyfile_singleton::instance();
    keyfiles.keydata_load();
    
    SelectKeyVC *dialog = [[SelectKeyVC alloc] initWithFrame:NSMakeRect(0, 0, 900, 540)];
    [dialog runModal];
    if ([[PassSyncStore sharedInstance] buttonClickType] == ButtonClickTypeOK) {
        return [[PassSyncStore sharedInstance] public_key];
    }
    
    return  dev::Public();
}

bool gui_mod_mac::entropy() const
{
    [ApplicationShared sharedInstance];
    EntropyCreatingVC *dialog = [[EntropyCreatingVC alloc] initWithFrame:NSMakeRect(0, 0, 700, 540)];
    [dialog runModal];
    if ([[PassSyncStore sharedInstance] buttonClickType] == ButtonClickTypeOK) {
        return true;
    } else {
        return false;
    }
}

