//
//  EntropyCreatingVC.m
//  keychain
//
//  Created by Mikhail Lutskiy on 07/03/2019.
//

#import "EntropyCreatingVC.h"
#import "LogoView.h"
#import "HexToRgbColor.h"
#import "SYFlatButton.h"
#include <keychain_lib/keychain.hpp>
#include <keychain_lib/keydata_singleton.hpp>
#include "PassSyncStore.h"
#import "VerticallyCenteredTextFieldCell.h"
#import "NSWindowController+extension.h"
//#include "keydata_singleton.hpp"

@interface EntropyCreatingVC () {
    NSView *zeroSceneView;
    NSView *firstSceneView;
    NSView *secondSceneView;
    NSView *thirdSceneView;
    NSView *fourthSceneView;
    NSTimer *timerSecond;
    NSTextField *nameField;
    NSTextField *descriptionField;
    NSSecureTextField *passwordField;
    NSSecureTextField *rePasswordField;
    std::string seed;
    std::string masterPassword;
}

@end

@implementation EntropyCreatingVC

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
    [NSApp setActivationPolicy:NSApplicationActivationPolicyProhibited];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSApp stopModal];
    });
}

- (void)runModal {
    [LogoView setLogoViewForWindow:self.window withTitle:@"Protecting your keys with entropy"];
    [[self getFirstView] setHidden:false];
//    [self setupLogoView];
//    [self createTableView];
//    [self setupCancelButton];
//    [self setupOkButton];
    [[NSApplication sharedApplication] runModalForWindow:self.window];
    
    [self.window setFrame:NSMakeRect(0, 0, 575, 500) display:true];
}

- (NSView *) getZeroView {
    if (!zeroS)
}

- (NSView *) getFirstView {
    if (!firstSceneView) {
        firstSceneView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, self.window.frame.size.width, 400)];
        [self.window.contentView addSubview:firstSceneView];
        
        NSTextField *label = [NSTextField labelWithString:@"We use entropy as the first step to protecting your keys. Please make as much noise as you possibly can and move your mouse vigorously."];
//        label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
        label.font = [NSFont systemFontOfSize:16];
        label.frame = NSMakeRect(64, self.window.frame.size.height - 200, self.window.frame.size.width - 128, 54);
        label.lineBreakMode = NSLineBreakByWordWrapping;
        label.alignment = NSTextAlignmentCenter;
        [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [firstSceneView addSubview:label];
        
        NSTextField *label2 = [NSTextField labelWithString:@"We hope you have fun while we collect entropy data to confuse anyone who might want to access your keys."];
//        label2.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        label2.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
        label2.font = [NSFont systemFontOfSize:16];
        label2.frame = NSMakeRect(64, 30, self.window.frame.size.width - 128, 54);
        label2.lineBreakMode = NSLineBreakByWordWrapping;
        label2.alignment = NSTextAlignmentCenter;
        [label2 setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [firstSceneView addSubview:label2];
        
        NSTextField *label3 = [NSTextField labelWithString:@"Entropy generation..."];
//        label3.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        label3.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
        label3.font = [NSFont systemFontOfSize:14];
        label3.frame = NSMakeRect(64, 180, self.window.frame.size.width - 128, 54);
        label3.lineBreakMode = NSLineBreakByWordWrapping;
        label3.alignment = NSTextAlignmentCenter;
        [label3 setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [firstSceneView addSubview:label3];
        
        NSProgressIndicator *indicator = [[NSProgressIndicator alloc] initWithFrame:NSMakeRect(64, 240, self.window.frame.size.width - 128, 20)];
        indicator.style = NSProgressIndicatorStyleBar;
        indicator.minValue = 0;
        indicator.indeterminate = false;
        indicator.maxValue = 100.0;
        indicator.doubleValue = 0.0;
        NSLog(@"indicator");
        NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.5 repeats:true block:^(NSTimer * _Nonnull timer) {
            NSLog(@"timer");
            indicator.doubleValue += 10.0;
            if (indicator.doubleValue == 100) {
                [timer invalidate];
                NSLog(@"1234");
                [[self getFirstView] setHidden:true];
                [[self getSecondView] setHidden:false];
            }
        }];
        [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];

        [firstSceneView addSubview:indicator];
    }
    return firstSceneView;
}

- (NSView *) getSecondView {
    if (!secondSceneView) {
        secondSceneView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, self.window.frame.size.width, 400)];
        [self.window.contentView addSubview:secondSceneView];
        
        NSTextField *label = [NSTextField labelWithString:@"Congratulations!\nYour keys are protected with entropy.\n\nMemorize the seed displayed below. You will need it to restore your keys!\n\nThis message will self-destruct in 3 minutes and the seed will not be shown again for security reasons."];
//        label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
        label.font = [NSFont systemFontOfSize:16];
        label.frame = NSMakeRect(64, self.window.frame.size.height - 350, self.window.frame.size.width - 128, 200);
        label.lineBreakMode = NSLineBreakByWordWrapping;
        label.alignment = NSTextAlignmentCenter;
        [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [secondSceneView addSubview:label];
        
        dev::bytes value = {0, 35, 35 ,38};
        
//        auto& key_data = keychain_app::keydata_singleton::instance();
        dev::bytes user_entropy;
        auto mnemonics = keychain_app::keydata::seed(user_entropy);
        seed = mnemonics;
//        auto& keyfiles = keychain_app::keyfile_singleton::instance();
//        auto seed = keyfiles.seed_phrase(value);
        
        __block NSTextField *label2 = [NSTextField labelWithString:[NSString stringWithUTF8String:mnemonics.c_str()]];
//        label2.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        label2.textColor = [NSColor redColor];
        label2.font = [NSFont systemFontOfSize:27];
        label2.frame = NSMakeRect(32, 30, self.window.frame.size.width - 64, 100);
        label2.lineBreakMode = NSLineBreakByWordWrapping;
        label2.alignment = NSTextAlignmentCenter;
        [label2 setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [secondSceneView addSubview:label2];
        
        NSTextField *label3 = [NSTextField labelWithString:@"180 seconds"];
//        label3.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        label3.textColor = [NSColor blackColor];
        label3.font = [NSFont boldSystemFontOfSize:22];
        label3.frame = NSMakeRect(64, 160, self.window.frame.size.width - 128, 54);
        label3.lineBreakMode = NSLineBreakByWordWrapping;
        label3.alignment = NSTextAlignmentCenter;
        [label3 setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [secondSceneView addSubview:label3];
        
        SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 125, 20, 100, 35)];
        button.target = self;
        button.action = @selector(nextButtonClick);
        button.title = @"NEXT";
        button.titleNormalColor = [NSColor whiteColor];
        button.momentary = YES;
        button.cornerRadius = 4.0;
        button.backgroundNormalColor = [HexToRgbColor colorWithHexColorString:@"4686FF"];
        [secondSceneView addSubview:button];
        
        __block SYFlatButton *buttonPauseResume = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 245, 20, 100, 35)];
        buttonPauseResume.target = self;
        buttonPauseResume.action = @selector(pauseResumeButton);
        buttonPauseResume.title = @"PAUSE";
        buttonPauseResume.backgroundNormalColor = [NSColor whiteColor];
        buttonPauseResume.titleNormalColor = [HexToRgbColor colorWithHexColorString:@"939497"];
        buttonPauseResume.cornerRadius = 4;
        buttonPauseResume.momentary = YES;
        [secondSceneView addSubview:buttonPauseResume];
        
        __block int timerSec = 180;
        
        timerSecond = [NSTimer scheduledTimerWithTimeInterval:1 repeats:true block:^(NSTimer * _Nonnull timer) {
            NSLog(@"timer");
            timerSec -= 1;
            label3.stringValue = [NSString stringWithFormat:@"%d seconds", timerSec];
            
            if (timerSec == 0) {
                label2.stringValue = @"Time is up";
                [buttonPauseResume setHidden:true];
                [timer invalidate];
                NSLog(@"1234");
//                [[self getSecondView] setHidden:true];
            }
        }];
        [[NSRunLoop currentRunLoop] addTimer:timerSecond forMode:NSRunLoopCommonModes];
        
    }
    return secondSceneView;
}

- (NSView *) getThirdView {
    if (!thirdSceneView) {
        thirdSceneView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, self.window.frame.size.width, 400)];
        [self.window.contentView addSubview:thirdSceneView];
        
        NSTextField *label = [NSTextField labelWithString:@"Create Master key"];
//        label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        label.textColor = [HexToRgbColor colorWithHexColorString:@"000000"];
        label.font = [NSFont systemFontOfSize:20];
        label.frame = NSMakeRect(64, self.window.frame.size.height - 200, self.window.frame.size.width - 128, 54);
        label.lineBreakMode = NSLineBreakByWordWrapping;
        label.alignment = NSTextAlignmentCenter;
        [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [thirdSceneView addSubview:label];
        
        /* Create Labels */
//
//        NSTextField *nameLabel = [NSTextField labelWithString:@"Name"];
////        nameLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        nameLabel.frame = NSMakeRect(22, 300, 130, 30);
//        nameLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Name"];
//        nameLabel.cell.font = [NSFont systemFontOfSize:18];
//        [thirdSceneView addSubview:nameLabel];
//
//        NSTextField *descriptionLabel = [NSTextField labelWithString:@"Description"];
////        descriptionLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        descriptionLabel.frame = NSMakeRect(22, 262, 130, 30);
//        descriptionLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Description"];
//        descriptionLabel.cell.font = [NSFont systemFontOfSize:18];
//        [thirdSceneView addSubview:descriptionLabel];
//
//        NSTextField *encryptedLabel = [NSTextField labelWithString:@"Encrypted"];
////        encryptedLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        encryptedLabel.frame = NSMakeRect(22, 224, 130, 30);
//        encryptedLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Encrypted"];
//        encryptedLabel.cell.font = [NSFont systemFontOfSize:18];
//        [thirdSceneView addSubview:encryptedLabel];
//
//        NSTextField *cipherLabel = [NSTextField labelWithString:@"Cipher"];
////        cipherLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        cipherLabel.frame = NSMakeRect(22, 186, 130, 30);
//        cipherLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Cipher"];
//        cipherLabel.cell.font = [NSFont systemFontOfSize:18];
//        [thirdSceneView addSubview:cipherLabel];
        
        NSTextField *passwordLabel = [NSTextField labelWithString:@"Password"];
//        passwordLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        passwordLabel.frame = NSMakeRect(22, 148, 130, 30);
        passwordLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Password"];
        passwordLabel.cell.font = [NSFont systemFontOfSize:18];
        [thirdSceneView addSubview:passwordLabel];
        
        NSTextField *rePasswordLabel = [NSTextField labelWithString:@"Re-password"];
//        rePasswordLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        rePasswordLabel.frame = NSMakeRect(22, 110, 130, 30);
        rePasswordLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Re-password"];
        rePasswordLabel.cell.font = [NSFont systemFontOfSize:18];
        [thirdSceneView addSubview:rePasswordLabel];
        
        /* Create fields */
//
//        nameField = [[NSTextField alloc] initWithFrame:CGRectMake(150, 300, self.window.frame.size.width - 180, 30)];
//        nameField.backgroundColor = [NSColor whiteColor];
//        nameField.font = [NSFont systemFontOfSize:20];
//        nameField.layer.cornerRadius = 4.0;
////        nameField.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        [thirdSceneView addSubview:nameField];
//
//        descriptionField = [[NSTextField alloc] initWithFrame:CGRectMake(150, 262, self.window.frame.size.width - 180, 30)];
//        descriptionField.backgroundColor = [NSColor whiteColor];
//        descriptionField.font = [NSFont systemFontOfSize:20];
//        descriptionField.layer.cornerRadius = 4.0;
////        descriptionField.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
//        [thirdSceneView addSubview:descriptionField];
        
        passwordField = [[NSSecureTextField alloc] initWithFrame:CGRectMake(150, 148, self.window.frame.size.width - 180, 30)];
        passwordField.backgroundColor = [NSColor whiteColor];
        passwordField.font = [NSFont systemFontOfSize:20];
        passwordField.layer.cornerRadius = 4.0;
//        passwordField.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        [thirdSceneView addSubview:passwordField];
        
        rePasswordField = [[NSSecureTextField alloc] initWithFrame:CGRectMake(150, 110, self.window.frame.size.width - 180, 30)];
        rePasswordField.backgroundColor = [NSColor whiteColor];
        rePasswordField.font = [NSFont systemFontOfSize:20];
        rePasswordField.layer.cornerRadius = 4.0;
//        rePasswordField.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        [thirdSceneView addSubview:rePasswordField];
        
//        nameField.nextKeyView = descriptionField;
//        descriptionField.nextKeyView = passwordField;
        passwordField.nextKeyView = rePasswordField;
        rePasswordField.nextKeyView = passwordField;

        /* Create buttons */
        
        SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 125, 20, 100, 35)];
        button.target = self;
        button.action = @selector(createMasterKey);
        button.title = @"CREATE";
        button.titleNormalColor = [NSColor whiteColor];
        button.momentary = YES;
        button.cornerRadius = 4.0;
        button.backgroundNormalColor = [HexToRgbColor colorWithHexColorString:@"4686FF"];
        [thirdSceneView addSubview:button];
        
        SYFlatButton *buttonCancelResume = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 245, 20, 100, 35)];
        buttonCancelResume.target = self;
        buttonCancelResume.action = @selector(closeButtonClick);
        buttonCancelResume.title = @"CANCEL";
        buttonCancelResume.backgroundNormalColor = [NSColor whiteColor];
        buttonCancelResume.titleNormalColor = [HexToRgbColor colorWithHexColorString:@"939497"];
        buttonCancelResume.cornerRadius = 4;
        buttonCancelResume.momentary = YES;
        [thirdSceneView addSubview:buttonCancelResume];
    }
    return thirdSceneView;
}

- (NSView *) getFourthView {
    if (!fourthSceneView) {
        fourthSceneView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, self.window.frame.size.width, 400)];
        [self.window.contentView addSubview:fourthSceneView];
        
        NSTextField *label = [NSTextField labelWithString:@"Create key"];
        //        label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        //        label.textColor = [HexToRgbColor colorWithHexColorString:@"000000"];
        label.font = [NSFont systemFontOfSize:20];
        label.frame = NSMakeRect(64, self.window.frame.size.height - 200, self.window.frame.size.width - 128, 54);
        label.lineBreakMode = NSLineBreakByWordWrapping;
        label.alignment = NSTextAlignmentCenter;
        [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [fourthSceneView addSubview:label];
        
        /* Create Labels */
        
        NSTextField *nameLabel = [NSTextField labelWithString:@"Name"];
        //        nameLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        nameLabel.frame = NSMakeRect(22, 300, 130, 30);
        nameLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Name"];
        nameLabel.cell.font = [NSFont systemFontOfSize:18];
        [fourthSceneView addSubview:nameLabel];
        
        NSTextField *descriptionLabel = [NSTextField labelWithString:@"Description"];
        //        descriptionLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        descriptionLabel.frame = NSMakeRect(22, 262, 130, 30);
        descriptionLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Description"];
        descriptionLabel.cell.font = [NSFont systemFontOfSize:18];
        [fourthSceneView addSubview:descriptionLabel];
        
        NSTextField *encryptedLabel = [NSTextField labelWithString:@"Encrypted"];
        //        encryptedLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        encryptedLabel.frame = NSMakeRect(22, 224, 130, 30);
        encryptedLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Encrypted"];
        encryptedLabel.cell.font = [NSFont systemFontOfSize:18];
        [fourthSceneView addSubview:encryptedLabel];
        
        NSTextField *cipherLabel = [NSTextField labelWithString:@"Cipher"];
        //        cipherLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        cipherLabel.frame = NSMakeRect(22, 186, 130, 30);
        cipherLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Cipher"];
        cipherLabel.cell.font = [NSFont systemFontOfSize:18];
        [fourthSceneView addSubview:cipherLabel];
        
        NSTextField *passwordLabel = [NSTextField labelWithString:@"Password"];
        //        passwordLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        passwordLabel.frame = NSMakeRect(22, 148, 130, 30);
        passwordLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Password"];
        passwordLabel.cell.font = [NSFont systemFontOfSize:18];
        [fourthSceneView addSubview:passwordLabel];
        
        NSTextField *rePasswordLabel = [NSTextField labelWithString:@"Re-password"];
        //        rePasswordLabel.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        rePasswordLabel.frame = NSMakeRect(22, 110, 130, 30);
        rePasswordLabel.cell = [[VerticallyCenteredTextFieldCell alloc] initTextCell:@"Re-password"];
        rePasswordLabel.cell.font = [NSFont systemFontOfSize:18];
        [fourthSceneView addSubview:rePasswordLabel];
        
        /* Create fields */
        
        nameField = [[NSTextField alloc] initWithFrame:CGRectMake(150, 300, self.window.frame.size.width - 180, 30)];
        nameField.backgroundColor = [NSColor whiteColor];
        nameField.font = [NSFont systemFontOfSize:20];
        nameField.layer.cornerRadius = 4.0;
        //        nameField.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        [fourthSceneView addSubview:nameField];
        
        descriptionField = [[NSTextField alloc] initWithFrame:CGRectMake(150, 262, self.window.frame.size.width - 180, 30)];
        descriptionField.backgroundColor = [NSColor whiteColor];
        descriptionField.font = [NSFont systemFontOfSize:20];
        descriptionField.layer.cornerRadius = 4.0;
        //        descriptionField.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        [fourthSceneView addSubview:descriptionField];
        
        passwordField = [[NSSecureTextField alloc] initWithFrame:CGRectMake(150, 148, self.window.frame.size.width - 180, 30)];
        passwordField.backgroundColor = [NSColor whiteColor];
        passwordField.font = [NSFont systemFontOfSize:20];
        passwordField.layer.cornerRadius = 4.0;
        //        passwordField.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        [fourthSceneView addSubview:passwordField];
        
        rePasswordField = [[NSSecureTextField alloc] initWithFrame:CGRectMake(150, 110, self.window.frame.size.width - 180, 30)];
        rePasswordField.backgroundColor = [NSColor whiteColor];
        rePasswordField.font = [NSFont systemFontOfSize:20];
        rePasswordField.layer.cornerRadius = 4.0;
        //        rePasswordField.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        [fourthSceneView addSubview:rePasswordField];
        
        nameField.nextKeyView = descriptionField;
        descriptionField.nextKeyView = passwordField;
        passwordField.nextKeyView = rePasswordField;
        
        /* Create buttons */
        
        SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 125, 20, 100, 35)];
        button.target = self;
        button.action = @selector(createKey);
        button.title = @"CREATE";
        button.titleNormalColor = [NSColor whiteColor];
        button.momentary = YES;
        button.cornerRadius = 4.0;
        button.backgroundNormalColor = [HexToRgbColor colorWithHexColorString:@"4686FF"];
        [fourthSceneView addSubview:button];
        
        SYFlatButton *buttonCancelResume = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 245, 20, 100, 35)];
        buttonCancelResume.target = self;
        buttonCancelResume.action = @selector(closeButtonClick);
        buttonCancelResume.title = @"CANCEL";
        buttonCancelResume.backgroundNormalColor = [NSColor whiteColor];
        buttonCancelResume.titleNormalColor = [HexToRgbColor colorWithHexColorString:@"939497"];
        buttonCancelResume.cornerRadius = 4;
        buttonCancelResume.momentary = YES;
        [fourthSceneView addSubview:buttonCancelResume];
        
    }
    return fourthSceneView;
}

- (void) pauseResumeButton {
    [timerSecond invalidate];
}

- (void) nextButtonClick {
    [[self getFirstView] setHidden:true];
    [[self getSecondView] setHidden:true];
    [[self getThirdView] setHidden:false];
//    [[PassSyncStore sharedInstance] setButtonClickType: ButtonClickTypeOK];
//    [self.window close];
}

- (void) createMasterKey {
    if ([passwordField.stringValue isEqualToString:rePasswordField.stringValue] && ![passwordField.stringValue isEqualToString:@""]) {
        masterPassword = std::string([passwordField.stringValue UTF8String]);
        keychain_app::keydata::derive_masterkey(seed, masterPassword);
        
        [[self getFirstView] setHidden:true];
        [[self getSecondView] setHidden:true];
        [[self getThirdView] setHidden:true];
        [[self getFourthView] setHidden:false];
    } else {
        [self showAlertWithTitle:@"Error" andText:@"Passwords must match and not be empty" withCompletion:nil];
    }
}

- (void) createKey {
    if (![nameField.stringValue isEqualToString:@""]) {
        if ([passwordField.stringValue isEqualToString:rePasswordField.stringValue] && ![passwordField.stringValue isEqualToString:@""]) {
            std::string passWordKey = [passwordField.stringValue UTF8String];
            keychain_app::keydata::path_levels_t path;
            path.root="m";
            path.purpose=44;
            path.coin_type=0;
            path.account=0;
            path.change=0;
            path.address_index=0;
            
            keychain_app::keydata::create_t cmd;
            cmd.keyname = [nameField.stringValue UTF8String];
            cmd.description = [descriptionField.stringValue UTF8String];
            cmd.encrypted = true;
            cmd.cipher = keychain_app::keyfile_format::cipher_etype::aes256;
            cmd.curve = keychain_app::keyfile_format::curve_etype::secp256k1;
            cmd.password = passWordKey;
            cmd.path = fc_light::variant(path);
            
            auto json = fc_light::json::to_string(cmd);
            
//            NSString *json = [NSString stringWithFormat:@"{\"keyname\":\"%@\", \"description\":\"%@\",\"encrypted\":true,\"cipher\":\"aes256\",\"curve\":\"secp256k1\",\"password\":\"%@\",\"path\":{\"root\":\"m\",\"purpose\"0,\"coin_type\":0,\"account\":0,\"change\":0,\"address_index\":0}}",nameField.stringValue,descriptionField.stringValue,passwordField.stringValue];
//            json = [json stringByReplacingOccurrencesOfString:@"" withString:@""];
//            std::string jsonString = json;
            keychain_app::keydata::derive_key(masterPassword, json);
//            auto pass = [passwordField.stringValue UTF8String];
//            //    typedef typename decltype(pass)::print_type print;
//
//            auto& keyfiles = keychain_app::keyfile_singleton::instance();
//
//            keyfiles.create(std::bind(&keychain_app::create_new_keyfile, [nameField.stringValue UTF8String], [descriptionField.stringValue UTF8String], true, keychain_app::keyfile_format::cipher_etype::aes256, keychain_app::keyfile_format::curve_etype::secp256k1, [&pass](const std::string& keyname) {
//                std::vector<char> result;
//                std::copy(pass, pass + strlen(pass), std::back_inserter(result));
//                return result;
//            }));
            [self showAlertWithTitle:@"Congratulations" andText:@"Your first key was created successfully." withCompletion:^{
                [[PassSyncStore sharedInstance] setButtonClickType: ButtonClickTypeOK];
                [self.window close];
            }];
            
        } else {
            [self showAlertWithTitle:@"Error" andText:@"Passphrases must match and not be empty" withCompletion:nil];
        }
    } else {
        [self showAlertWithTitle:@"Error" andText:@"Keyname must not be empty" withCompletion:nil];
    }

}

- (void) closeButtonClick {
    [[PassSyncStore sharedInstance] setButtonClickType: ButtonClickTypeCancel];
    [self.window close];
}

@end
