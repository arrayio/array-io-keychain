//
//  ApplicationShared.mm
//  keychain
//
//  Created by Mikhail Lutskiy on 27/11/2018.
//

#import "ApplicationShared.h"

@implementation ApplicationShared

+ (instancetype)sharedInstance
{
    static ApplicationShared *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ApplicationShared alloc] init];
    });
    return sharedInstance;
}

- (instancetype)init
{
    self = [super init];
    if (self) {
        [self setAppMenu];
    }
    return self;
}

- (void) setAppMenu {
    [NSApplication sharedApplication];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    id menubar = [NSMenu new];
    id appMenuItem = [NSMenuItem new];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];
    id appMenu = [NSMenu new];
    id appName = [[NSProcessInfo processInfo] processName];
    id quitTitle = @"Quit Keychain";
    id quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                  action:@selector(terminate:) keyEquivalent:@"q"];
    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];
    
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp finishLaunching];
}

@end
