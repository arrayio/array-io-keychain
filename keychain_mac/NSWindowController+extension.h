//
//  NSWindowController+extension.h
//  keychain
//
//  Created by Mikhail Lutskiy on 14/03/2019.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSWindowController (NSWindowController_extension)

- (void) showAlertWithTitle:(NSString *) title andText:(NSString *) text withCompletion: (void (^ __nullable)(void)) completion;

@end

NS_ASSUME_NONNULL_END
