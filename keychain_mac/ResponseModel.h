//
//  ResponseModel.h
//  keychain
//
//  Created by Mikhail Lutskiy on 28/11/2018.
//

#import "JSONModel/JSONModel.h"
#import "DataModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface ResponseModel : JSONModel

@property (nonatomic) BOOL json;
@property (nonatomic) NSString *blockchain;
@property (nonatomic) NSString *protocol;
@property (nonatomic) DataModel *data;

@end

NS_ASSUME_NONNULL_END
