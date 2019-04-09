//
//  RestoreVC.mm
//  keychain
//
//  Created by Mikhail Lutskiy on 08/04/2019.
//

#import "RestoreVC.h"
#import "LogoView.h"
#import "HexToRgbColor.h"
#import "SYFlatButton.h"
#include <keychain_lib/keychain.hpp>
#include <keychain_lib/keydata_singleton.hpp>
#include "PassSyncStore.h"
#import "VerticallyCenteredTextFieldCell.h"
#import "NSWindowController+extension.h"

@interface RestoreVC ()

@end

@implementation RestoreVC

- (instancetype)initWithFrame:(NSRect)frame {
    NSWindowStyleMask windowMask = NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView;
    NSWindow *window = [[NSWindow alloc] initWithContentRect:frame
                                                   styleMask:windowMask
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
    [window setTitle:@"KeyChain"];
    [window makeKeyAndOrderFront:self];
    [window orderFront:self];
    
    [NSNotificationCenter.defaultCenter addObserver:self
                                           selector:@selector(windowWillClose:)
                                               name:NSWindowWillCloseNotification
                                             object:nil];
    window.titlebarAppearsTransparent = YES;
    //window.titleVisibility = NSWindowTitleHidden;
    //    window.backgroundColor = [NSColor whiteColor];
    [window center];
    return [super initWithWindow:window];
}

- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
}

- (void)windowWillClose:(NSNotification *)notification {
    NSLog(@"windowWillClose");
    [NSApp setActivationPolicy:NSApplicationActivationPolicyProhibited];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSApp stopModal];
    });
}

- (void)runModal {
    [LogoView setLogoViewForWindow:self.window withTitle:@"Restore"];
//    [[self getZeroView] setHidden:false];
    //    [self setupLogoView];
    //    [self createTableView];
    //    [self setupCancelButton];
    //    [self setupOkButton];
    [[NSApplication sharedApplication] runModalForWindow:self.window];
    
    [self.window setFrame:NSMakeRect(0, 0, 575, 500) display:true];
}


@end
