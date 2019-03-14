//
//  VerticallyCenteredTextFieldCell.m
//  keychain
//
//  Created by Mikhail Lutskiy on 14/03/2019.
//

#import "VerticallyCenteredTextFieldCell.h"

@implementation VerticallyCenteredTextFieldCell

- (NSRect) titleRectForBounds:(NSRect)frame {
    
    CGFloat stringHeight = self.attributedStringValue.size.height;
    NSRect titleRect = [super titleRectForBounds:frame];
    CGFloat oldOriginY = frame.origin.y;
    titleRect.origin.y = frame.origin.y + (frame.size.height - stringHeight) / 2.0;
    titleRect.size.height = titleRect.size.height - (titleRect.origin.y - oldOriginY);
    return titleRect;
}

- (void) drawInteriorWithFrame:(NSRect)cFrame inView:(NSView*)cView {
    [super drawInteriorWithFrame:[self titleRectForBounds:cFrame] inView:cView];
}

@end
