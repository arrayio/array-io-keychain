//
//  ApplicationShared.mm
//  keychain
//
//  Created by Mikhail Lutskiy on 27/11/2018.
//

#import "ApplicationShared.h"
#import "PassSyncStore.h"
#import "FileManager.h"

@implementation ApplicationShared

+ (instancetype)sharedInstance
{
    static ApplicationShared *sharedInstance = nil;
    static dispatch_once_t onceToken;
    [NSApplication sharedApplication];
    [NSWorkspace.sharedWorkspace setIcon:[[NSImage alloc] initWithContentsOfFile:[NSString stringWithFormat:@"%@/resources/logo.png", FileManager.getWorkDirectoryPath]] forFile:FileManager.getBinaryPath options:0];
    [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
    [[PassSyncStore sharedInstance] setPass:@""];
    dispatch_once(&onceToken, ^{
        sharedInstance = [[ApplicationShared alloc] init];
        [NSApp setDelegate:self];
    });
    [NSApp activateIgnoringOtherApps:YES];
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
    id menubar = [NSMenu new];
    id appMenuItem = [NSMenuItem new];
    [menubar addItem:appMenuItem];
    [NSApp setMainMenu:menubar];
    id appMenu = [NSMenu new];
    id quitTitle = @"Quit KeyChain";
    id quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle
                                                  action:@selector(terminate:) keyEquivalent:@"q"];
    [appMenu addItem:quitMenuItem];
    [appMenuItem setSubmenu:appMenu];
    [NSApp finishLaunching];
}

@end
