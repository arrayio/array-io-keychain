//
//  LogoView.h
//  keychain
//
//  Created by Mikhail Lutskiy on 07/03/2019.
//

//#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
NS_ASSUME_NONNULL_BEGIN

@interface LogoView : NSObject

+ (void)setLogoViewForWindow:(NSWindow*)window;
+ (void)setLogoViewForWindow:(NSWindow *)window withTitle:(NSString *) title;

@end

NS_ASSUME_NONNULL_END
