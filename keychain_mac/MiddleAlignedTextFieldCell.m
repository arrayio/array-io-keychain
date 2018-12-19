//
//  MiddleAlignedTextFieldCell.m
//  keychain
//
//  Created by Mikhail Lutskiy on 19/12/2018.
//

#import "MiddleAlignedTextFieldCell.h"

@implementation NSTextFieldCell (MiddleAlignedTextFieldCell)

- (void)setVerticalCentering:(BOOL)centerVertical
{
    @try { _cFlags.vCentered = centerVertical ? 1 : 0; }
    @catch(...) { NSLog(@"*** unable to set vertical centering"); }
}

@end
