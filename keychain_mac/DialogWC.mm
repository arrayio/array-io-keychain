//
//  DialogWC.m
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//

#import "DialogWC.h"
#import "PassSyncStore.h"
#import "SYFlatButton.h"
#import "HexToRgbColor.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#import "FileManager.h"
#include <keychain_lib/secmod_parser_cmd.hpp>

using keychain_app::secmod_commands::secmod_parser_f;

@interface DialogWC () {
    NSSecureTextField *pass;
    NSSecureTextField *passConfirm;
}

@end

@implementation DialogWC

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
        [alert setIcon:[NSImage new]];
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
    if (!self.unlockOnly) {
        if (_isSignTransaction) {
            [self checkForRedLock];
            [self setupLogoBlockhain:self.blockhainType];
            [self setupTo];
            [self setupFrom];
            [self setupAmount];
            [self setupExpertModeButton];
            if (self.isJson) {
                
                secmod_parser_f cmd_parse;
                auto cmd_type = cmd_parse([self.jsonString UTF8String]);
                
                switch (cmd_type)
                {
                    case keychain_app::secmod_commands::blockchain_secmod_te::unknown:
                    {
                        
                    }
                        break;
                    case keychain_app::secmod_commands::blockchain_secmod_te::ethereum:
                    {
                        auto eth_trx = cmd_parse.to_ethereum();
                        auto eth_data = eth_trx.trx_info;
                        [self setupTextTo:[NSString stringWithUTF8String:eth_data.to.c_str()]];
                        [self setupTextFrom:[NSString stringWithUTF8String:eth_trx.from.c_str()]];
                        [self setupTextAmount:[NSString stringWithUTF8String:eth_data.value.c_str()]];
                    }
                        break;
                    case keychain_app::secmod_commands::blockchain_secmod_te::bitcoin:
                    {
                        
                    }
                        break;
                    case keychain_app::secmod_commands::blockchain_secmod_te::rawhash:
                    {
                        
                    }
                        break;
                    case keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap:
                    {
                        auto swap_trx = cmd_parse.to_ethereum_swap();
                        auto swap_info = swap_trx.swap_info;
                        switch (swap_info.action)
                        {
                            case keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::secmod_command_swap::action_te::create_swap:
                                [self setupSwapAdditional:@"Hash"];
                                [self setupTextSwapAdditional:[NSString stringWithUTF8String:swap_info.hash.c_str()]];
                                [self setupTextSwapAction:@"Create Swap"];
                                break;
                            case keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::secmod_command_swap::action_te::refund:
                                [self setupSwapAdditional:@""];
                                [self setupTextSwapAdditional:@""];
                                [self setupTextSwapAction:@"Refund"];
                                break;
                            case keychain_app::secmod_commands::secmod_command<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>::secmod_command_swap::action_te::withdraw:
                                [self setupSwapAdditional:@"Secret"];
                                [self setupTextSwapAdditional:[NSString stringWithUTF8String:swap_info.secret.c_str()]];
                                [self setupTextSwapAction:@"Withdraw"];
                                break;
                        }
                        auto eth_data = swap_trx.trx_info;
                        [self setupTextTo:[NSString stringWithUTF8String:eth_data.to.c_str()]];
                        [self setupTextFrom:[NSString stringWithUTF8String:swap_trx.from.c_str()]];
                        [self setupTextAmount:[NSString stringWithUTF8String:eth_data.value.c_str()]];
                        [self setupSwapAddress];
                        [self setupSwapAction];
                        [self setupLogoSwap];
                        [self setupTextSwapAddress:[NSString stringWithUTF8String:swap_info.address.c_str()]];
                    }
                        break;
                    case keychain_app::secmod_commands::blockchain_secmod_te::parse_error:
                    {
                        
                    }
                        break;
                    default:
                    {
                        
                    }
                        break;
                }
            }
        } else {
            [self setupTitleLabel:@"Enter the password for the new key"];
            [self setupPassConfirmField];
            [self setupLabelConfirmPassphrase];
            [self setupRecommendationTextForPassword];
        }
    } else {
        [self checkForRedLock];
        [self setupTitleLabel:@"Unlock"];
    }
    [[NSApplication sharedApplication] runModalForWindow:self.window];
    
    [self.window setFrame:NSMakeRect(0, 0, 575, 500) display:true];
}

- (void) checkForRedLock {
    if (self.unlockTime > 0 || !self.isJson) {
        [self setupLogoRedLock];
    }
}

- (void) setupLogoRedLock {
    NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 41, self.window.frame.size.height - 65, 19, 25)];
    NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/locked-padlock.png"];
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
    NSLog(@"path %@", path);
    imageView.image = image;
    [self.window.contentView addSubview:imageView];
}

- (void) setupLogoBlockhain:(NSString *)blockhain {
    if ([blockhain isEqualToString:@"ethereum"]) {
        NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(22, 224, 25, 39)];
        NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/ethereum.png"];
        NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
        NSLog(@"path %@", path);
        imageView.image = image;
        [self.window.contentView addSubview:imageView];
    } else if ([blockhain isEqualToString:@"bitcoun"]) {
        NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(22, 224, 25, 35)];
        NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/bitcoin.png"];
        NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
        NSLog(@"path %@", path);
        imageView.image = image;
        [self.window.contentView addSubview:imageView];
    }
}

- (void) setupLogoSwap {
    NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(22, 382, 50, 28)];
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
    NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(22, self.window.frame.size.height - 81, 64, 54)];
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
    label.frame = NSMakeRect(22, 147, 100, 30);
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
    passConfirm = [[NSSecureTextField alloc] initWithFrame:CGRectMake(130, 150, self.window.frame.size.width - 155, 30)];
    passConfirm.backgroundColor = [NSColor whiteColor];
    passConfirm.font = [NSFont systemFontOfSize:20];
    passConfirm.layer.cornerRadius = 4.0;
    passConfirm.nextKeyView = pass;
    passConfirm.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [self.window.contentView addSubview:passConfirm];
}

- (void) setupRecommendationTextForPassword {
    NSTextField *label = [NSTextField labelWithString:@"* We recommend to use more than 13 characters, to combine lowercase and uppercase letters, digits and symbols for extra safety (a-z, A-Z, 0-9, @#$%*)"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:12];
    label.frame = NSMakeRect(130, 104, 423, 45);
    label.lineBreakMode = NSLineBreakByWordWrapping;
    [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
    [self.window.contentView addSubview:label];
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
    [alert setIcon:[NSImage new]];
    alert.messageText = @"Expert mode";
    alert.informativeText = self.expertModeString;
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
