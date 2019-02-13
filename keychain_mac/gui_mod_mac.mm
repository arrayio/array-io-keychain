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

using namespace keychain_app;

gui_mod_mac::gui_mod_mac()
{}

gui_mod_mac::~gui_mod_mac()
{}

dev::Public gui_mod_mac::select_key() const
{
    [ApplicationShared sharedInstance];
    SelectKeyVC *dialog = [[SelectKeyVC alloc] initWithFrame:NSMakeRect(0, 0, 700, 540)];
    [dialog runModal];
    if ([[PassSyncStore sharedInstance] buttonClickType] == ButtonClickTypeOK) {
        return [[PassSyncStore sharedInstance] public_key];
    }
    
    return  dev::Public();
}
