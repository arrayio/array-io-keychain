//
//  NSWindowController+extension.m
//  keychain
//
//  Created by Mikhail Lutskiy on 14/03/2019.
//

#import "NSWindowController+extension.h"

@implementation NSWindowController (NSWindowController_extension)

- (void) showAlertWithTitle:(NSString *) title andText:(NSString *) text withCompletion: (void (^ __nullable)(void)) completion  {
    NSAlert *alert = [NSAlert new];
    alert.icon = [NSImage new];
    alert.messageText = title;
    alert.informativeText = text;
    [alert addButtonWithTitle:@"OK"];
    [alert beginSheetModalForWindow:self.window completionHandler:^(NSModalResponse returnCode) {
        if (completion != nil)
            completion();
    }];
}

@end
