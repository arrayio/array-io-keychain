//
//  Key.h
//  KeyManager
//
//  Created by Mikhail Lutskiy on 14/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface Key : NSObject

@property (nonatomic, strong) NSString *name;
@property (nonatomic, strong) NSString *descriptionKey;
@property (nonatomic, strong) NSString *publicKey;
@property (nonatomic, strong) NSDate *createTime;
@property (nonatomic, strong) NSString *keychainVersion;
@property (nonatomic, strong) NSString *cipherType;

@end

NS_ASSUME_NONNULL_END
