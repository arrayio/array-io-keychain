//
//  MyDialog.m
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//

#import "MyDialog.h"
#import "PassSyncStore.h"

@interface MyDialog () {
    NSSecureTextField *pass;
}

@end

@implementation MyDialog

- (instancetype)initWithFrame:(NSRect)frame {
    NSWindowStyleMask windowMask = NSWindowStyleMaskTitled
    | NSWindowStyleMaskClosable;
    NSWindow *window = [[NSWindow alloc] initWithContentRect:frame
                                                   styleMask:windowMask
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
    [window setTitle:@"Keychain"];
    [window makeKeyAndOrderFront:self];
    [window orderFront:self];

    [NSNotificationCenter.defaultCenter addObserver:self
                                           selector:@selector(windowWillClose:)
                                               name:NSWindowWillCloseNotification
                                             object:nil];
    
    return [super initWithWindow:window];
}

- (void) clickButton {
    [[PassSyncStore sharedInstance] setPass:pass.stringValue];
    [self.window close];
}

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
}

- (void)runModal {
    pass = [[NSSecureTextField alloc] initWithFrame:CGRectMake(50, 100, 100, 25)];
    pass.placeholderString = @"Password";
    [self.window.contentView addSubview:pass];
    NSButton *button = [NSButton buttonWithTitle:@"Sign" target:self action:@selector(clickButton)];
    [self.window.contentView addSubview:button];
    [[NSApplication sharedApplication] runModalForWindow:self.window];
}

- (void)windowWillClose:(NSNotification *)notification {
    [NSApp setActivationPolicy:NSApplicationActivationPolicyProhibited];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSApp stopModal];
    });
}

@end
