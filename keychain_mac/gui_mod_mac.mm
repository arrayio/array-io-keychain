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

using namespace keychain_app;

gui_mod_mac::gui_mod_mac()
{}

gui_mod_mac::~gui_mod_mac()
{}

dev::Public gui_mod_mac::select_key() const
{
    [ApplicationShared sharedInstance];
    NSRect frame = NSMakeRect(0, 0, 500, 521);

    auto& keyfiles = keyfile_singleton::instance();
    auto it = keyfiles.begin();
    if ( it==keyfiles.end() )
    return dev::Public();
    
    SelectKeyVC *dialog = [[SelectKeyVC alloc] initWithFrame:frame];
    [dialog runModal];
    
    return it->keyinfo.public_key;
}
