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
#import "keychain-Swift.h"
#import "MiddleAlignedTextFieldCell.h"

using namespace keychain_app;

using keychain_app::secmod_commands::secmod_parser_f;

@interface DialogWC () <NSTableViewDelegate, NSTableViewDataSource> {
    NSSecureTextField *pass;
    NSSecureTextField *passConfirm;
    NSButton *redLockButton;
    BOOL isHash;
    NSMutableArray *dataForBitcoin;
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
    //window.titleVisibility = NSWindowTitleHidden;
    window.backgroundColor = [NSColor whiteColor];
    [window center];
    return [super initWithWindow:window];
}

- (void) clickCloseButton {
    [[PassSyncStore sharedInstance] setButtonClickType: ButtonClickTypeCancel];
    [self.window close];
}

- (void) clickButton {
    if (!self.isSignTransaction && ![pass.stringValue isEqualToString:passConfirm.stringValue]) {
        NSAlert *alert = [NSAlert new];
        [alert setIcon:[NSImage new]];
        alert.messageText = @"Error";
        alert.informativeText = @"Pa";
        [alert addButtonWithTitle:@"OK"];
        [alert beginSheetModalForWindow:[self.window.contentView window] completionHandler:^(NSInteger result) {
            NSLog(@"Success");
        }];
    } else {
        [[PassSyncStore sharedInstance] setButtonClickType: ButtonClickTypeOK];
        if (self.isPasswordRequire)
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
    if (self.isPasswordRequire) {
        [self setupLabelPassphrase];
        [self setupPassField];
    }
    [self setupSignButton];
    [self setupCancelButton];
    if (!self.unlockOnly) {
        if (_isSignTransaction) {
            [self setupLogoBlockhain:self.blockhainType];
            [self setupExpertModeButton];
            [self setupTitleLabel:[NSString stringWithFormat:@"You are trying to sign a transaction with the key \"%@\". %@", self.keyname, (self.unlockTime > 0) ? @"" : @""]];
            if (self.etype == secmod_commands::events_te::sign_hash) {
                [self.window setTitle:@"Sign hash"];
                isHash = true;
                auto cmd = self.parser.params<secmod_commands::events_te::sign_hash>();
                [self setupBottomLabel:@"Hash"];
                [self setupTextBottomLabel:[NSString stringWithUTF8String:cmd.hash.c_str()]];
                [self setupFrom];
                [self setupTextFrom:[NSString stringWithUTF8String:cmd.from.c_str()]];
            } else if (self.etype == secmod_commands::events_te::sign_hex) {
                auto cmd = self.parser.params<secmod_commands::events_te::sign_hex>();
                switch (cmd.blockchain) {
                    case secmod_commands::blockchain_secmod_te::unknown: {
                        NSLog(@"unknown");
                        [self.window setTitle:@"Sign hex"];
                        [self setupBottomLabel:@"Data"];

                        [self setupTextBottomLabel:[NSString stringWithUTF8String:cmd.get_trx_view<secmod_commands::blockchain_secmod_te::unknown>().c_str()]];
                        break;
                    }
                    case secmod_commands::blockchain_secmod_te::ethereum: {
                        NSLog(@"ethereum");
                        [self.window setTitle:@"Sign transaction"];
                        auto eth_trx = cmd.get_trx_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum>();
                        auto eth_data = eth_trx.trx_info;
                        [self setupBottomLabel:@"To"];
                        [self setupFrom];
                        [self setupTopLabel:@"Amount"];
                        [self setupTextBottomLabel:[NSString stringWithUTF8String:eth_data.to.c_str()]];
                        [self setupTextFrom:[NSString stringWithUTF8String:eth_trx.from.c_str()]];
                        [self setupTextTopLabel:[NSString stringWithUTF8String:eth_data.value.c_str()]];
                        break;
                    }
                    case secmod_commands::blockchain_secmod_te::ethereum_swap: {
                        NSLog(@"ethereum_swap");
                        [self.window setTitle:@"Sign transaction"];
                        [self setupBottomLabel:@"To"];
                        [self setupFrom];
                        [self setupTopLabel:@"Amount"];
    
                        auto swap_trx = cmd.get_trx_view<keychain_app::secmod_commands::blockchain_secmod_te::ethereum_swap>();
                        auto swap_info = swap_trx.swap_info;
                        switch (swap_info.action)
                        {
                            case keychain_app::secmod_commands::transaction_view<secmod_commands::blockchain_secmod_te::ethereum_swap>::secmod_command_swap::action_te::create_swap:
                                [self setupSwapAdditional:@"Hash"];
                                [self setupTextSwapAdditional:[NSString stringWithUTF8String:swap_info.hash.c_str()]];
                                [self setupTextSwapAction:@"Create Swap"];
                                break;
                            case keychain_app::secmod_commands::transaction_view<secmod_commands::blockchain_secmod_te::ethereum_swap>::secmod_command_swap::action_te::refund:
                                [self setupSwapAdditional:@""];
                                [self setupTextSwapAdditional:@""];
                                [self setupTextSwapAction:@"Refund"];
                                break;
                            case keychain_app::secmod_commands::transaction_view<secmod_commands::blockchain_secmod_te::ethereum_swap>::secmod_command_swap::action_te::withdraw:
                                [self setupSwapAdditional:@"Secret"];
                                [self setupTextSwapAdditional:[NSString stringWithUTF8String:swap_info.secret.c_str()]];
                                [self setupTextSwapAction:@"Withdraw"];
                                break;
                        }
                        auto eth_data = swap_trx.trx_info;
                        [self setupTextBottomLabel:[NSString stringWithUTF8String:eth_data.to.c_str()]];
                        [self setupTextFrom:[NSString stringWithUTF8String:swap_trx.from.c_str()]];
                        [self setupTextTopLabel:[NSString stringWithUTF8String:eth_data.value.c_str()]];
                        [self setupSwapAddress:@"Address"];
                        [self setupSwapAction];
                        [self setupLogoSwap];
                        [self setupTextSwapAddress:[NSString stringWithUTF8String:swap_info.address.c_str()]];
                        break;
                    }
                    case secmod_commands::blockchain_secmod_te::bitcoin: {
                        NSLog(@"bitcoin");
                        [self.window setTitle:@"Sign transaction"];
                        if (cmd.is_parsed) {
                            dataForBitcoin = [NSMutableArray new];
                            auto btc_trx = cmd.get_trx_view<keychain_app::secmod_commands::blockchain_secmod_te::bitcoin>();
                            for (int i = 0; i < btc_trx.trx_info.num_vouts; i++) {
                                auto vout = btc_trx.trx_info.vouts[i];
                                NSDictionary *dict = @{@"key1": [NSString stringWithUTF8String:vout.address.c_str()], @"key2": [NSString stringWithFormat:@"%llu", vout.amount]};
                                [dataForBitcoin addObject:dict];
                            }
                            [self setupSwapAddress:@"From"];
                            [self setupTextSwapAddress:[NSString stringWithUTF8String:btc_trx.from.c_str()]];
                            [self createTableView];
                        }
                        break;
                    }
                }
            }
        } else {
            [self.window setTitle:@"Create password"];
            [self setupTitleLabel:@"Enter the password for the new key"];
            [self setupPassConfirmField];
            [self setupLabelConfirmPassphrase];
            [self setupRecommendationTextForPassword];
        }
    } else {
        [self.window setTitle:@"Unlock private key"];
//        [self checkForRedLock];
        [self setupTitleLabel:[NSString stringWithFormat:@"You are trying to unlock the key \"%@\" for \"%d\" seconds.", self.keyname, self.unlockTime]];
    }
    [self checkForRedLock];
    [[NSApplication sharedApplication] runModalForWindow:self.window];
    
    [self.window setFrame:NSMakeRect(0, 0, 575, 500) display:true];
}

- (void) createTableView {
    NSScrollView *scrollView = [[NSScrollView alloc] initWithFrame:CGRectMake(22, 110, self.window.frame.size.width - 44, 150)];
    scrollView.backgroundColor = [NSColor clearColor];
    [scrollView setBorderType:NSBezelBorder];
    
    NSTableView *tableView = [[NSTableView alloc] initWithFrame:scrollView.bounds];
    tableView.rowSizeStyle = NSTableViewRowSizeStyleLarge;
    tableView.backgroundColor = [NSColor clearColor];
    tableView.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];

    NSTableColumn *tCol;
    
    tCol = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"key1"]];
    id cell = [tCol dataCell];
    [cell setFont: [NSFont systemFontOfSize:18]];
    [[tCol headerCell] setStringValue:@"To"];
    [tCol setWidth:self.window.frame.size.width - 149];
    tCol.editable = false;
    [[tCol dataCell] setVerticalCentering:YES];
    [tableView addTableColumn:tCol];
    
    tCol = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"key2"]];
    cell = [tCol dataCell];
    [cell setFont: [NSFont systemFontOfSize:18]];
    [[tCol headerCell] setStringValue:@"Amount"];
    [tCol setWidth:100];
    tCol.editable = false;
    [[tCol dataCell] setVerticalCentering:YES];
    [tableView addTableColumn:tCol];
    
    [tableView setUsesAlternatingRowBackgroundColors:YES];
    [tableView setGridStyleMask:NSTableViewSolidVerticalGridLineMask];
    [tableView setRowHeight:23.0];
    [tableView setDelegate:self];
    [tableView setDataSource:self];
    [tableView setSelectionHighlightStyle:NSTableViewSelectionHighlightStyleNone];
    [tableView setAutoresizesSubviews:YES];
    
    scrollView.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [scrollView setHasVerticalScroller:YES];
    [scrollView setHasHorizontalScroller:NO];
    scrollView.horizontalScrollElasticity = NSScrollElasticityNone;
    [scrollView setAutoresizesSubviews:YES];
    [scrollView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
    [scrollView setAutohidesScrollers:false];
    [scrollView setScrollerStyle:NSScrollerStyleLegacy];
    [scrollView setDocumentView:tableView];
    [self.window.contentView addSubview:scrollView];
}

- (void) checkForRedLock {
    if (self.isSignTransaction && (self.unlockTime > 0 || !self.isJson || isHash)) {
        [self setupLogoRedLock];
    } else {
        [self setupLogoGreenLock];
    }
}

- (void) setupLogoGreenLock {
    NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/locked-padlock-green.png"];
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
    NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 41, self.window.frame.size.height - 65, 19, 25)];
    NSLog(@"path %@", path);
    imageView.image = image;
    [self.window.contentView addSubview:imageView];
    
    NSButton *redLockButton = [[NSButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 41, self.window.frame.size.height - 65, 19, 25)];
    [redLockButton setImage:[NSImage new]];
    [redLockButton setImagePosition:NSImageOnly];
    [redLockButton setBordered:NO];
    [redLockButton setAction:@selector(greenlockButtonClicked)];
    [self.window.contentView addSubview:redLockButton];
}

- (void) setupLogoRedLock {
    NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/locked-padlock.png"];
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
    NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 41, self.window.frame.size.height - 65, 19, 25)];
    NSLog(@"path %@", path);
    imageView.image = image;
    [self.window.contentView addSubview:imageView];

    NSButton *redLockButton = [[NSButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 41, self.window.frame.size.height - 65, 19, 25)];
    [redLockButton setImage:[NSImage new]];
    [redLockButton setImagePosition:NSImageOnly];
    [redLockButton setBordered:NO];
    [redLockButton setAction:@selector(redlockButtonClicked)];
    [self.window.contentView addSubview:redLockButton];
}

- (void) greenlockButtonClicked {
    NSString *string;
    if (_isSignTransaction) {
        string = @"This transaction is successfully parsed by the core module.\nNo threats detected. You can now review the transaction details.";
    } else {
        string = @"Your private key is encrypted and secure.";
    }
    NSPopover *popover = [[NSPopover alloc] initWithContent:string doesAnimate:true];
    [popover showRelativeToRect:NSMakeRect(self.window.frame.size.width - 41, self.window.frame.size.height - 65, 19, 25) ofView:self.window.contentView.superview preferredEdge:NSRectEdgeMinY];
}

- (void) redlockButtonClicked {
    NSString *string = @"";
    if (self.unlockTime > 0) {
        string = [NSString stringWithFormat:@"%@Experimental function \"unlock key\" will be activated once you enter the passphrase.\nDuring this time operations with the unlocked key will be executed without user confirmation.", string];
    }
    if (!self.isJson && !self.unlockOnly) {
        string = [NSString stringWithFormat:@"%@%@KeyChain can provide only hex view of the transaction without additional information, such as address, amount, and any other detail.\nConfim the transaction only if you trust the app requesting the signature.", string, ([string isEqualToString:@""]) ? @"" : @"\n\n"];
    }
    if (isHash) {
        string = [NSString stringWithFormat:@"%@%@Using transaction hash does not provide any information about the transaction.\nUse the transaction hash only if you trust the app requesting the signature.", string, ([string isEqualToString:@""]) ? @"" : @"\n\n"];
    }
    NSPopover *popover = [[NSPopover alloc] initWithContent:string doesAnimate:true];
    [popover showRelativeToRect:NSMakeRect(self.window.frame.size.width - 41, self.window.frame.size.height - 65, 19, 25) ofView:self.window.contentView.superview preferredEdge:NSRectEdgeMinY];
}

- (void) setupLogoBlockhain:(NSString *)blockhain {
    if ([blockhain isEqualToString:@"ethereum"]) {
        NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(22, 224, 25, 39)];
        NSString *path = [NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/ethereum.png"];
        NSImage *image = [[NSImage alloc] initWithContentsOfFile:path];
        NSLog(@"path %@", path);
        imageView.image = image;
        [self.window.contentView addSubview:imageView];
    } else if ([blockhain isEqualToString:@"bitcoin"]) {
        NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(22, self.window.frame.size.height - 135, 25, 35)];
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
    label.frame = NSMakeRect(190, self.window.frame.size.height - 90, self.window.frame.size.width - 231, 54);
    label.lineBreakMode = NSLineBreakByWordWrapping;
    [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
    
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
    NSTextField *label = [NSTextField labelWithString:(!self.unlockOnly && !self.isSignTransaction) ? @"Confirm" : @"Passphrase"];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 67, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupBottomLabel:(NSString*) string {
    NSTextField *label = [NSTextField labelWithString:string];
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

- (void) setupTopLabel:(NSString *) string {
    NSTextField *label = [NSTextField labelWithString:string];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:18];
    label.frame = NSMakeRect(22, 187, 100, 30);
    [self.window.contentView addSubview:label];
}

- (void) setupSwapAddress:(NSString *) string {
    NSTextField *label = [NSTextField labelWithString:string];
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

- (void) setupTextBottomLabel:(NSString*)string {
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
    label.font = [NSFont systemFontOfSize:([string length] > 64) ? 12 : 18];
    label.frame = NSMakeRect(130, 147, self.window.frame.size.width - 150, 30);
    label.lineBreakMode = NSLineBreakByWordWrapping;
    [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
    [self.window.contentView addSubview:label];
}

- (void) setupTextTopLabel:(NSString*)string {
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
//    [pass.cell setAllowedInputSourceLocales:@[NSAllRomanInputSourcesLocaleIdentifier]];
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

- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
    // NSString *aString = [NSString stringWithFormat:@"%@, Row %ld",[aTableColumn identifier],(long)rowIndex];
    NSString *aString;
    aString = [[dataForBitcoin objectAtIndex:rowIndex] objectForKey:[aTableColumn identifier]];
    return aString;
}

// TableView Datasource method implementation
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
{
    //we have only one table in the screen and thus we are not checking the row count based on the target table view
//    long recordCount = [self.dataArray count];
    return dataForBitcoin.count;
}

- (NSString *)getCurrentKeyboardLanguage {
    
}

@end
