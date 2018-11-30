//
//  DataModel.h
//  keychain
//
//  Created by Mikhail Lutskiy on 28/11/2018.
//

#import "JSONModel/JSONModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface DataModel : JSONModel

@property (nonatomic) NSString <Optional> *from;
@property (nonatomic) NSString <Optional> *to;
@property (nonatomic) NSString <Optional> *value;
@property (nonatomic) NSString <Optional> *nonce;
@property (nonatomic) NSString <Optional> *gas;
@property (nonatomic) NSString <Optional> *gasPrice;
@property (nonatomic) NSInteger chainid;

@end

NS_ASSUME_NONNULL_END
