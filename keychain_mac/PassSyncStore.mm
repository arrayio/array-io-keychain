//
//  PassSyncStore.mm
//  keychain
//
//  Created by Mikhail Lutskiy on 31/10/2018.
//

#import "PassSyncStore.h"

@implementation PassSyncStore

+ (instancetype)sharedInstance
{
    static PassSyncStore *sharedInstance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedInstance = [[PassSyncStore alloc] init];
    });
    return sharedInstance;
}

@end
