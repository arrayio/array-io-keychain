//
//  DataModel.h
//  keychain
//
//  Created by Mikhail Lutskiy on 28/11/2018.
//

#import "JSONModel/JSONModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface DataModel : JSONModel

@property (nonatomic) NSString *from;
@property (nonatomic) NSString *to;
@property (nonatomic) NSString *value;

@end

NS_ASSUME_NONNULL_END
