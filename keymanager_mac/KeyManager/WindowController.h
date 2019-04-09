//
//  WindowController.h
//  KeyManager
//
//  Created by Mikhail Lutskiy on 14/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface WindowController : NSWindowController
@property (weak) IBOutlet NSToolbarItem *restoreButton;
- (IBAction)createKeyAction:(id)sender;
@property (weak) IBOutlet NSToolbarItem *removeKeyItem;
- (IBAction)removeKeyAction:(id)sender;
- (IBAction)backupAction:(id)sender;
- (IBAction)restoreAction:(id)sender;

@end

NS_ASSUME_NONNULL_END
