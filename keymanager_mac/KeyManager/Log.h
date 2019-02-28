//
//  Log.h
//  KeyManager
//
//  Created by Mikhail Lutskiy on 15/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, BlockchainType) {
    BlockchainTypeArray,
    BlockchainTypeUnknown,
    BlockchainTypeBitshares,
    BlockchainTypeEthereum,
    BlockchainTypeBitcoin,
    BlockchainTypeRawhash
};

NS_ASSUME_NONNULL_BEGIN

@interface Log : NSObject

@property BlockchainType blockchainType;
@property (nonatomic, strong) NSString* chainId;
@property (nonatomic, strong) NSDate* signTime;
@property (nonatomic, strong) NSString* transaction;

@end

NS_ASSUME_NONNULL_END
