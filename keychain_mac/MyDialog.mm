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
#import "ResponseModel.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#import "keychain-Swift.h"

@interface MyDialog () {
    NSSecureTextField *pass;
    NSSecureTextField *passConfirm;
}

@end

@implementation MyDialog

- (instancetype)initWithFrame:(NSRect)frame {
    NSWindowStyleMask windowMask = NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView | NSWindowStyleMaskClosable;
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
    window.titleVisibility = NSWindowTitleHidden;
    window.backgroundColor = [NSColor whiteColor];
    [window center];
    return [super initWithWindow:window];
}

- (void) clickCloseButton {
    [self.window close];
}

- (void) clickButton {
    if (!self.isSignTransaction && ![pass.stringValue isEqualToString:passConfirm.stringValue]) {
        NSAlert *alert = [NSAlert new];
        alert.messageText = @"Error";
        alert.informativeText = @"Passphrases must match";
        [alert addButtonWithTitle:@"OK"];
        [alert beginSheetModalForWindow:[self.window.contentView window] completionHandler:^(NSInteger result) {
            NSLog(@"Success");
        }];
    } else {
        [[PassSyncStore sharedInstance] setPass:pass.stringValue];
        [self.window close];
    }
}

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
}

- (void)runModal {
    NSVisualEffectView *cover = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, self.window.frame.size.width, self.window.frame.size.height - 91)];
    cover.blendingMode = NSVisualEffectBlendingModeBehindWindow;
    cover.material = NSVisualEffectMaterialAppearanceBased;
    cover.state = NSVisualEffectStateFollowsWindowActiveState;
    [cover setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameVibrantLight]];
    [self.window.contentView addSubview:cover];
    NSLog(@"jsonString %@", _jsonString);
    NSLog(@"currentPath %@", _currentPath);
    [self setupLogoiew];
    [self setupLabelPassphrase];
    [self setupPassField];
    [self setupSignButton];
    [self setupCancelButton];
    if (_isSignTransaction) {
        NSError *error;
        ResponseModel *model = [[ResponseModel alloc] initWithString:self.jsonString error:&error];
        [self setupLogoBlockhain:model.blockchain];
        [self setupTo];
        [self setupFrom];
        [self setupAmount];
        [self setupExpertModeButton];
        if (error == nil) {
            [self setupTextTo:model.data.to];
            [self setupTextFrom:model.data.from];
            [self setupTextAmount:model.data.value];
            if (model.swap != NULL) {
                [self setupSwapButton];
                NSLog(@" null %@", model.swap);
            }
        } else {
            NSLog(@"Error %@", error);
        }
    } else {
        [self setupTitleLabel:@"Enter the password for the new key"];
        [self setupPassConfirmField];
        [self setupLabelConfirmPassphrase];
    }
    [[NSApplication sharedApplication] runModalForWindow:self.window];
}

- (void) setupLogoBlockhain:(NSString *)blockhain {
    if ([blockhain isEqualToString:@"ethereum"]) {
        ImageAspectView *imageView = [[ImageAspectView alloc] initWithFrame:NSMakeRect(22, 224, 25, 39)];
        NSString *path = [NSString stringWithFormat:@"%@/%@", self.currentPath, @"resources/ethereum.png"];
        NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
        NSLog(@"path %@", path);
        imageView.image = image;
        [self.window.contentView addSubview:imageView];
    } else if ([blockhain isEqualToString:@"bitcoun"]) {
        ImageAspectView *imageView = [[ImageAspectView alloc] initWithFrame:NSMakeRect(22, 224, 25, 35)];
        NSString *path = [NSString stringWithFormat:@"%@/%@", self.currentPath, @"resources/bitcoin.png"];
        NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
        NSLog(@"path %@", path);
        imageView.image = image;
        [self.window.contentView addSubview:imageView];
    }
}

- (void) setupTitleLabel:(NSString *)string {
    NSTextField *label = [NSTextField labelWithString:string];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:14];
    label.frame = NSMakeRect(190, self.window.frame.size.height - 98, 330, 54);
    
    [self.window.contentView addSubview:label];
}

- (void) setupLogoiew {
    
    ImageAspectView *imageView = [[ImageAspectView alloc] initWithFrame:NSMakeRect(22, self.window.frame.size.height - 81, 64, 54)];
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithFormat:@"%@/%@", self.currentPath, @"resources/logo.png"]];
    imageView.image = image;
    [self.window.contentView addSubview:imageView];
    
    NSTextField *label = [NSTextField labelWithString:@"KeyChain"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(90, self.window.frame.size.height - 96, 100, 54);
    
    [self.window.contentView addSubview:label];
}

- (void) setupLabelConfirmPassphrase {
    NSTextField *label = [NSTextField labelWithString:@"Passphrase"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 117, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupLabelPassphrase {
    NSTextField *label = [NSTextField labelWithString:self.isSignTransaction ? @"Passphrase" : @"Confirm"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 67, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTo {
    NSTextField *label = [NSTextField labelWithString:@"To"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 107, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupFrom {
    NSTextField *label = [NSTextField labelWithString:@"From"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 147, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupAmount {
    NSTextField *label = [NSTextField labelWithString:@"Amount"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 187, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextTo:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:string];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 107, 450, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextFrom:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:string];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 147, 450, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextAmount:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:string];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 187, 450, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupPassField {
    pass = [[NSSecureTextField alloc] initWithFrame:CGRectMake(130, 70, 420, 30)];
    pass.backgroundColor = [NSColor whiteColor];
    pass.font = [NSFont systemFontOfSize:20];
    pass.layer.cornerRadius = 4.0;
    pass.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [self.window.contentView addSubview:pass];
}

- (void) setupPassConfirmField {
    passConfirm = [[NSSecureTextField alloc] initWithFrame:CGRectMake(130, 120, 420, 30)];
    passConfirm.backgroundColor = [NSColor whiteColor];
    passConfirm.font = [NSFont systemFontOfSize:20];
    passConfirm.layer.cornerRadius = 4.0;
    passConfirm.nextKeyView = pass;
    passConfirm.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [self.window.contentView addSubview:passConfirm];
}

- (void) setupSignButton {
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(450, 20, 100, 35)];
    button.target = self;
    button.action = @selector(clickButton);
    button.title = self.isSignTransaction ? @"SIGN" : @"CREATE";
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

- (void) setupExpertModeButton {
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(22, 20, 100, 35)];
    button.target = self;
    button.action = @selector(expertModeAlert);
    button.title = @"EXPERT MODE";
    button.backgroundNormalColor = [NSColor whiteColor];
    button.titleNormalColor = [HexToRgbColor colorWithHexColorString:@"939497"];
    button.cornerRadius = 4;
    button.momentary = YES;
    [self.window.contentView addSubview:button];
}

- (void) setupSwapButton {
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(142, 20, 100, 35)];
    button.target = self;
    button.action = @selector(swapAlert);
    button.title = @"SWAP";
    button.backgroundNormalColor = [NSColor whiteColor];
    button.titleNormalColor = [HexToRgbColor colorWithHexColorString:@"939497"];
    button.cornerRadius = 4;
    button.momentary = YES;
    [self.window.contentView addSubview:button];
}


- (void) expertModeAlert {
    NSAlert *alert = [NSAlert new];
    alert.messageText = @"Expert mode";
    alert.informativeText = self.jsonString;
    [alert addButtonWithTitle:@"OK"];
    [alert beginSheetModalForWindow:[self.window.contentView window] completionHandler:^(NSInteger result) {
        NSLog(@"Success");
    }];
}

- (void) swapAlert {
    NSError *error;
    ResponseModel *model = [[ResponseModel alloc] initWithString:self.jsonString error:&error];

    NSAlert *alert = [NSAlert new];
    alert.messageText = @"Swap";
    alert.informativeText = model.swap.toJSONString;
    [alert addButtonWithTitle:@"OK"];
    [alert beginSheetModalForWindow:[self.window.contentView window] completionHandler:^(NSInteger result) {
        NSLog(@"Success");
    }];
}

- (void)windowWillClose:(NSNotification *)notification {
    [NSApp setActivationPolicy:NSApplicationActivationPolicyProhibited];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSApp stopModal];
    });
}

@end
