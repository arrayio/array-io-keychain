//
//  MyDialog.m
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//

#import "MyDialog.h"
#import "PassSyncStore.h"
#import "SYFlatButton.h"
#import "HexToRgbColor.h"

@interface MyDialog () {
    NSSecureTextField *pass;
}

@end

@implementation MyDialog

- (instancetype)initWithFrame:(NSRect)frame {
    NSWindowStyleMask windowMask = NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView | NSWindowStyleMaskClosable;
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
//    window.titleVisibility = NSWindowTitleHidden;
    window.titlebarAppearsTransparent = YES;
    [window center];
//    [window setBackgroundColor:[NSColor whiteColor]];
    return [super initWithWindow:window];
}

- (void) clickCloseButton {
    [self.window close];
}

- (void) clickButton {
    [[PassSyncStore sharedInstance] setPass:pass.stringValue];
    [self.window close];
}

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
}

- (void)runModal {
    NSVisualEffectView *cover = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, 575, 270)];
    cover.blendingMode = NSVisualEffectBlendingModeBehindWindow;
    cover.material = NSVisualEffectMaterialAppearanceBased;
    cover.state = NSVisualEffectStateFollowsWindowActiveState;
    [cover setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameVibrantLight]];
    [self.window.contentView addSubview:cover];
    [self setupLabelPassphrase];
    [self setupPassField];
    [self setupSignButton];
    [self setupCancelButton];
    [[NSApplication sharedApplication] runModalForWindow:self.window];
}

- (void) setupLabelPassphrase {
    NSTextField *label = [NSTextField labelWithString:@"Passphrase"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"434343"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 67, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupPassField {
    pass = [[NSSecureTextField alloc] initWithFrame:CGRectMake(130, 70, 320, 30)];
    pass.backgroundColor = [NSColor whiteColor];
    pass.font = [NSFont systemFontOfSize:20];
    pass.layer.cornerRadius = 4.0;
    pass.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [self.window.contentView addSubview:pass];
}

- (void) setupSignButton {
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(450, 20, 100, 35)];
    button.target = self;
    button.action = @selector(clickButton);
    button.title = @"SIGN";
    button.titleNormalColor = [NSColor whiteColor];
    button.momentary = YES;
    button.cornerRadius = 4.0;
    button.backgroundNormalColor = [HexToRgbColor colorWithHexColorString:@"4686FF"];
    [self.window.contentView addSubview:button];
}

- (void) setupCancelButton {
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(330, 20, 100, 35)];
    button.target = self;
    button.action = @selector(clickCloseButton);
    button.title = @"CANCEL";
    button.backgroundNormalColor = [NSColor whiteColor];
    button.titleNormalColor = [HexToRgbColor colorWithHexColorString:@"939497"];
    button.cornerRadius = 4;
    button.momentary = YES;
    [self.window.contentView addSubview:button];
}

- (void)windowWillClose:(NSNotification *)notification {
    [NSApp setActivationPolicy:NSApplicationActivationPolicyProhibited];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSApp stopModal];
    });
}

@end
