//
//  RestoreVC.h
//  keychain
//
//  Created by Mikhail Lutskiy on 08/04/2019.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface RestoreVC : NSWindowController

- (instancetype)initWithFrame:(NSRect)frame;
- (void)runModal;

@end

NS_ASSUME_NONNULL_END
