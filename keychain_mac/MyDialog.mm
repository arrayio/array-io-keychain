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
#import "FileManager.h"

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
//    NSLog(@"currentPath %@", _currentPath);
    [self setupLogoiew];
    [self setupLabelPassphrase];
    [self setupPassField];
    [self setupSignButton];
    [self setupCancelButton];
    if (_isSignTransaction) {
//        NSError *error;
//        ResponseModel *model = [[ResponseModel alloc] initWithString:self.jsonString error:&error];
        [self setupLogoBlockhain:self.jsonModel.blockchain];
        [self setupTo];
        [self setupFrom];
        [self setupAmount];
        [self setupExpertModeButton];
        if (!self.isRawTransaction) {
            [self setupTextTo:self.jsonModel.data.to];
            [self setupTextFrom:self.jsonModel.data.from];
            [self setupTextAmount:self.jsonModel.data.value];
            if (self.jsonModel.swap != NULL) {
                [self setupSwapAddress];
                if ([self.jsonModel.swap.action isEqualToString:@"createSwap"]) {
                    [self setupSwapAdditional:@"Hash"];
                    [self setupTextSwapAdditional:self.jsonModel.swap.hashVar];
                } else if ([self.jsonModel.swap.action isEqualToString:@"refund"]) {
                    [self setupSwapAdditional:@""];
                    [self setupTextSwapAdditional:@""];
                } else if ([self.jsonModel.swap.action isEqualToString:@"Withdraw"]) {
                    [self setupSwapAdditional:@"Secret"];
                    [self setupTextSwapAdditional:self.jsonModel.swap.secret];
                }
                [self setupSwapAction];
                [self setupLogoSwap];
                [self setupTextSwapAddress:self.jsonModel.swap.address];
                [self setupTextSwapAction:self.jsonModel.swap.action];
                NSLog(@" null %@", self.jsonModel.swap);
            }
        }
    } else {
        [self setupTitleLabel:@"Enter the password for the new key"];
        [self setupPassConfirmField];
        [self setupLabelConfirmPassphrase];
    }
    [[NSApplication sharedApplication] runModalForWindow:self.window];
    
    [self.window setFrame:NSMakeRect(0, 0, 575, 500) display:true];
}

- (void) setupLogoBlockhain:(NSString *)blockhain {
    if ([blockhain isEqualToString:@"ethereum"]) {
        ImageAspectView *imageView = [[ImageAspectView alloc] initWithFrame:NSMakeRect(22, 224, 25, 39)];
        NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/ethereum.png"];
        NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
        NSLog(@"path %@", path);
        imageView.image = image;
        [self.window.contentView addSubview:imageView];
    } else if ([blockhain isEqualToString:@"bitcoun"]) {
        ImageAspectView *imageView = [[ImageAspectView alloc] initWithFrame:NSMakeRect(22, 224, 25, 35)];
        NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/bitcoin.png"];
        NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
        NSLog(@"path %@", path);
        imageView.image = image;
        [self.window.contentView addSubview:imageView];
    }
}

- (void) setupLogoSwap {
    ImageAspectView *imageView = [[ImageAspectView alloc] initWithFrame:NSMakeRect(22, 382, 50, 28)];
    NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/swap logo.png"];
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
    NSLog(@"path %@", path);
    imageView.image = image;
    [self.window.contentView addSubview:imageView];
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
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/logo.png"]];
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

- (void) setupSwapAddress {
    NSTextField *label = [NSTextField labelWithString:@"Address"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 262, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextSwapAddress:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:[NSString stringWithFormat:@"%@", string]];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 262, self.window.frame.size.width - 150, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupSwapAdditional: (NSString *) string {
    NSTextField *label = [NSTextField labelWithString:[NSString stringWithFormat:@"%@", string]];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 302, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextSwapAdditional:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:[NSString stringWithFormat:@"%@", string]];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 302, self.window.frame.size.width - 150, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupSwapAction {
    NSTextField *label = [NSTextField labelWithString:@"Action"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 342, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextSwapAction:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:[NSString stringWithFormat:@"%@", string]];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 342, self.window.frame.size.width - 150, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextTo:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:string];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 107, self.window.frame.size.width - 150, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextFrom:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:string];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 147, self.window.frame.size.width - 150, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupTextAmount:(NSString*)string {
    NSTextField *label = [NSTextField labelWithString:string];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"7b8da7"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(130, 187, self.window.frame.size.width - 150, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupPassField {
    pass = [[NSSecureTextField alloc] initWithFrame:CGRectMake(130, 70, self.window.frame.size.width - 155, 30)];
    pass.backgroundColor = [NSColor whiteColor];
    pass.font = [NSFont systemFontOfSize:20];
    pass.layer.cornerRadius = 4.0;
    pass.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [self.window.contentView addSubview:pass];
}

- (void) setupPassConfirmField {
    passConfirm = [[NSSecureTextField alloc] initWithFrame:CGRectMake(130, 120, self.window.frame.size.width - 155, 30)];
    passConfirm.backgroundColor = [NSColor whiteColor];
    passConfirm.font = [NSFont systemFontOfSize:20];
    passConfirm.layer.cornerRadius = 4.0;
    passConfirm.nextKeyView = pass;
    passConfirm.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [self.window.contentView addSubview:passConfirm];
}

- (void) setupSignButton {
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 125, 20, 100, 35)];
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
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 245, 20, 100, 35)];
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
