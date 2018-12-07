//
//  ResponseModel.h
//  keychain
//
//  Created by Mikhail Lutskiy on 28/11/2018.
//

#import "JSONModel/JSONModel.h"
#import "DataModel.h"
#import "SwapModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface ResponseModel : JSONModel

@property (nonatomic) BOOL json;
@property (nonatomic) NSString *blockchain;
@property (nonatomic) NSString <Optional> *protocol;
@property (nonatomic) DataModel *data;
@property (nonatomic) SwapModel <Optional> *swap;

@end

NS_ASSUME_NONNULL_END
