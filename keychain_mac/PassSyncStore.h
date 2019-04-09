//
//  PassSyncStore.h
//  keychain
//
//  Created by Mikhail Lutskiy on 31/10/2018.
//

#import <Foundation/Foundation.h>
#import <keychain_lib/keyfile_parser.hpp>

NS_ASSUME_NONNULL_BEGIN

typedef enum {
    ButtonClickTypeOK,
    ButtonClickTypeCancel
} ButtonClickType;

@interface PassSyncStore : NSObject

@property (atomic, strong) NSString *pass;
@property dev::Public public_key;
@property ButtonClickType buttonClickType;
@property BOOL isRestore;
@property ButtonClickType buttonSelectFileType;
@property NSString *selectedFilePath;

+ (instancetype)sharedInstance;

@end

NS_ASSUME_NONNULL_END
