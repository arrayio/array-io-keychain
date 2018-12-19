//
//  MiddleAlignedTextFieldCell.h
//  keychain
//
//  Created by Mikhail Lutskiy on 19/12/2018.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSTextFieldCell (MiddleAlignedTextFieldCell)
    - (void)setVerticalCentering:(BOOL)centerVertical;
@end

NS_ASSUME_NONNULL_END
