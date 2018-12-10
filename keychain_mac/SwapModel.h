//
//  SwapModel.h
//  keychain
//
//  Created by Mikhail Lutskiy on 29/11/2018.
//

#import "JSONModel/JSONModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface SwapModel : JSONModel

@property (nonatomic) NSString *action;
@property (nonatomic) NSString <Optional> *address;
@property (nonatomic) NSString <Optional> *hashVar;
@property (nonatomic) NSString <Optional> *secret;


@end

NS_ASSUME_NONNULL_END
