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
@property (nonatomic) NSString *address;
@property (nonatomic) NSString *hashVar;
@property (nonatomic) NSString *secret;


@end

NS_ASSUME_NONNULL_END
