//
//  FileManager.h
//  keychain
//
//  Created by Mikhail Lutskiy on 03/12/2018.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface FileManager : NSObject

+ (NSString *) getWorkDirectoryPath;
+ (NSString *) getBinaryPath;

@end

NS_ASSUME_NONNULL_END
