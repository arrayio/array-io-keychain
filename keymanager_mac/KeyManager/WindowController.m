//
//  WindowController.m
//  KeyManager
//
//  Created by Mikhail Lutskiy on 14/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

#import "WindowController.h"
#import "KeyManager-Swift.h"

@interface WindowController ()

@end

@implementation WindowController

- (void)windowDidLoad {
    [super windowDidLoad];
    self.window.titlebarAppearsTransparent = true;
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
@end
