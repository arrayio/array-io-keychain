//
//  SwapModel.m
//  keychain
//
//  Created by Mikhail Lutskiy on 29/11/2018.
//

#import "SwapModel.h"

@implementation SwapModel

+ (JSONKeyMapper *)keyMapper
{
    return [[JSONKeyMapper alloc] initWithModelToJSONDictionary:@{
                                                                  @"action": @"action",
                                                                  @"hashVar": @"hash",
                                                                  @"address": @"address",
                                                                  @"secret": @"secret"
                                                                  }];
}

@end
