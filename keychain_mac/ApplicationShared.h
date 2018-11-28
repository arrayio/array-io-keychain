//
//  ApplicationShared.h
//  keychain
//
//  Created by Mikhail Lutskiy on 27/11/2018.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface ApplicationShared : NSObject

+ (instancetype)sharedInstance;

@end

NS_ASSUME_NONNULL_END
