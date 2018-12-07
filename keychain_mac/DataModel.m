//
//  DataModel.m
//  keychain
//
//  Created by Mikhail Lutskiy on 28/11/2018.
//

#import "DataModel.h"

@implementation DataModel

+ (BOOL)propertyIsOptional:(NSString *)propertyName
{
    if ([propertyName isEqualToString:@"chainid"])
        return YES;
    
    return YES;
}

@end
