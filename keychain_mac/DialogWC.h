//
//  DialogWC.h
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//

#import <Cocoa/Cocoa.h>
//#import "ResponseModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface DialogWC : NSWindowController

@property (nonatomic, strong) NSString *jsonString;
@property (nonatomic, strong) NSString *expertModeString;
@property (nonatomic, strong) NSString *blockhainType;
@property (readwrite) BOOL isSignTransaction;
//@property (nonatomic, strong) NSString *currentPath;
//@property (nonatomic, strong) ResponseModel *jsonModel;
@property (readwrite) BOOL isJson;
@property (readwrite) int unlockTime;
@property (readwrite) BOOL unlockOnly;

- (instancetype)initWithFrame:(NSRect)frame;
- (void)runModal;
@end

NS_ASSUME_NONNULL_END
