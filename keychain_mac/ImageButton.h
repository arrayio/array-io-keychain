//
//  ImageButton.h
//  keychain
//
//  Created by Mikhail Lutskiy on 09/04/2019.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSPopover (ImageButton)
- (id) initWithContent:(NSString *)content doesAnimate:(BOOL)animate;
@end

NS_ASSUME_NONNULL_END
