//
//  CPlusPlusBridger.h
//  KeyManager
//
//  Created by Mikhail Lutskiy on 13/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Key.h"
#import "Log.h"

NS_ASSUME_NONNULL_BEGIN

@interface CPlusPlusBridger : NSObject

- (NSInteger) getKeyfilesCount;
- (Key *) getKeyById:(NSInteger) indexRow;
- (void) createKeyWithName:(NSString* )keyname description:(NSString*)keyDescription encrypted:(BOOL)encrypted password:(NSString *) password cipher:(NSString *) cipher masterKey:(NSString *) masterKey blockchain:(int) blockhain;
- (void) reloadData;
- (NSMutableArray <Log *>*) getTransactionLogForPublicKey:(NSString *) publicKey;
- (void) backup:(NSString *) path;
- (void) restore:(NSString *) path seed:(NSString *)seed pass:(NSString *) pass;

@end

NS_ASSUME_NONNULL_END
