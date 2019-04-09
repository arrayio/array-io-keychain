//
//  WindowController.m
//  KeyManager
//
//  Created by Mikhail Lutskiy on 14/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

#import "WindowController.h"
#import "KeyManager-Swift.h"
#import "CPlusPlusBridger.h"
//#include <keychain_lib/keychain.hpp>

@interface WindowController ()

@end

@implementation WindowController

- (void)windowDidLoad {
    [super windowDidLoad];
    if ([[CPlusPlusBridger alloc] getKeyfilesCount] > 0) {
        [_restoreButton setEnabled:false];
    } else {
        if ([[[NSProcessInfo processInfo] arguments] containsObject:@"RESTORE"]) {
            NSOpenPanel *dialog = [NSOpenPanel new];
            dialog.title = @"Choose file";
            dialog.showsResizeIndicator    = true;
            dialog.showsHiddenFiles        = false;
            dialog.canChooseDirectories    = false;
            dialog.canCreateDirectories    = true;
            dialog.canChooseFiles          = true;
            dialog.allowsMultipleSelection = false;
            
            if (dialog.runModal == NSModalResponseOK) {
                NSURL *result = dialog.URL;
                if (result != nil) {
                    RestoreVC *vc = [self.storyboard instantiateControllerWithIdentifier:@"RestoreVC"];
                    vc.pathFile = [result.path stringByReplacingOccurrencesOfString:@"file://" withString:@""];
                    vc.needCancelSpecial = true;
                    vc.title = @"KeyManager";
                    [self.contentViewController presentViewControllerAsModalWindow:vc];
                    [_restoreButton setEnabled:false];
                }
            } else {
                [AppleScriptManager runScriptWithBody:[[[NSBundle mainBundle] bundlePath] stringByReplacingOccurrencesOfString:@"KeyManager.app" withString:@"keychain"] isAdminRequired:false success:^{
                    
                } failure:^(NSDictionary * _Nonnull d) {
                    NSLog(@"%@", d);
                }];
                [[NSApplication sharedApplication] terminate:self];
            }
        } else {
//            NSFileManager *filemgr;
//            NSString *currentpath;
//
//            filemgr = [[NSFileManager alloc] init];
//
//            currentpath = [filemgr currentDirectoryPath];
//
//            [AppleScriptManager runScriptWithBodyWithReturnString:@"pwd" isAdminRequired:false success:^(NSString * _Nonnull d) {
//                NSLog(@"%@", d);
//                NSAlert *alert = [NSAlert new];
//                alert.messageText = [NSString stringWithFormat:@"param = %@ %@", d, [[[NSBundle mainBundle] bundlePath] stringByReplacingOccurrencesOfString:@"KeyManager.app" withString:"keycahin"]];
//                [alert beginSheetModalForWindow:self.window completionHandler:nil];
//            } failure:^(NSDictionary * _Nonnull d) {
//                NSLog(@"%@", d);
//                NSAlert *alert = [NSAlert new];
//                alert.messageText = [NSString stringWithFormat:@"param = %@", d];
//                [alert beginSheetModalForWindow:self.window completionHandler:nil];
//            }];
            [AppleScriptManager runScriptWithBody:[[[NSBundle mainBundle] bundlePath] stringByReplacingOccurrencesOfString:@"KeyManager.app" withString:@"keychain"] isAdminRequired:false success:^{
                
            } failure:^(NSDictionary * _Nonnull d) {
                NSLog(@"%@", d);
//                NSAlert *alert = [NSAlert new];
//                alert.messageText = [NSString stringWithFormat:@"param = %@", d];
//                [alert beginSheetModalForWindow:self.window completionHandler:nil];
            }];
            [[NSApplication sharedApplication] terminate:self];
        }
//        NSLog(@"param = %@", [[NSUserDefaults standardUserDefaults] valueForKey:@"Param"]);
//        NSAlert *alert = [NSAlert new];
//        alert.messageText = [NSString stringWithFormat:@"param = %@", [[NSProcessInfo processInfo] arguments]];
//        [alert beginSheetModalForWindow:self.window completionHandler:nil];
    }
//    self.window.titlebarAppearsTransparent = true;
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

- (IBAction)createKeyAction:(id)sender {
    CreateKeyVC *vc = [self.storyboard instantiateControllerWithIdentifier:@"CreateKeyVC"];
    [self.contentViewController presentViewControllerAsSheet:vc];
}
- (IBAction)removeKeyAction:(id)sender {
    DeleteKeyVC *vc = [self.storyboard instantiateControllerWithIdentifier:@"DeleteKeyVC"];
    [self.contentViewController presentViewControllerAsSheet:vc];
}

- (IBAction)backupAction:(id)sender {
    NSOpenPanel *dialog = [NSOpenPanel new];
    dialog.title = @"Choose directory";
    dialog.showsResizeIndicator    = true;
    dialog.showsHiddenFiles        = false;
    dialog.canChooseDirectories    = true;
    dialog.canCreateDirectories    = true;
    dialog.canChooseFiles          = false;
    dialog.allowsMultipleSelection = false;
    
    if (dialog.runModal == NSModalResponseOK) {
        NSURL *result = dialog.URL;
        if (result != nil) {
            NSDate *now = [NSDate new];
            NSDateFormatter *df = [NSDateFormatter new];
            df.dateFormat = @"yyyy-MM-dd HH:mm:ss";
            [[CPlusPlusBridger alloc] backup:[NSString stringWithFormat:@"%@/kydata_%@.backup", [result.path stringByReplacingOccurrencesOfString:@"file://" withString:@""], [df stringFromDate:now]]];
        }
    }
//    keychain_app::keydata::backup("");
}

- (IBAction)restoreAction:(id)sender {
    NSOpenPanel *dialog = [NSOpenPanel new];
    dialog.title = @"Choose file";
    dialog.showsResizeIndicator    = true;
    dialog.showsHiddenFiles        = false;
    dialog.canChooseDirectories    = false;
    dialog.canCreateDirectories    = true;
    dialog.canChooseFiles          = true;
    dialog.allowsMultipleSelection = false;
    
    if (dialog.runModal == NSModalResponseOK) {
        NSURL *result = dialog.URL;
        if (result != nil) {
            RestoreVC *vc = [self.storyboard instantiateControllerWithIdentifier:@"RestoreVC"];
            vc.pathFile = [result.path stringByReplacingOccurrencesOfString:@"file://" withString:@""];
            [self.contentViewController presentViewControllerAsSheet:vc];
            [_restoreButton setEnabled:false];
        }
    }
}

@end
