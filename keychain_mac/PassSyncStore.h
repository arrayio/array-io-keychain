//
//  PassSyncStore.h
//  keychain
//
//  Created by Mikhail Lutskiy on 31/10/2018.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PassSyncStore : NSObject

@property (atomic, strong) NSString *pass;

+ (instancetype)sharedInstance;
@end

NS_ASSUME_NONNULL_END
