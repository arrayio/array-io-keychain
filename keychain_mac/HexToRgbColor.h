//
//  RGBColor.h
//  keychain
//
//  Created by Mikhail Lutskiy on 26/11/2018.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface HexToRgbColor : NSColor

+ (NSColor*)colorWithHexColorString:(NSString*)inColorString;

@end

NS_ASSUME_NONNULL_END
