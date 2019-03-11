//
//  EntropyCreatingVC.h
//  keychain
//
//  Created by Mikhail Lutskiy on 07/03/2019.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface EntropyCreatingVC : NSWindowController

- (instancetype)initWithFrame:(NSRect)frame;
- (void)runModal;

@end

NS_ASSUME_NONNULL_END
