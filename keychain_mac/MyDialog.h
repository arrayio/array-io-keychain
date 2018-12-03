//
//  MyDialog.h
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//

#import <Cocoa/Cocoa.h>
#import "ResponseModel.h"

NS_ASSUME_NONNULL_BEGIN

@interface MyDialog : NSWindowController

@property (nonatomic, strong) NSString *jsonString;
@property (readwrite) BOOL isSignTransaction;
//@property (nonatomic, strong) NSString *currentPath;
@property (nonatomic, strong) ResponseModel *jsonModel;
@property (readwrite) BOOL isRawTransaction;

- (instancetype)initWithFrame:(NSRect)frame;
- (void)runModal;
@end

NS_ASSUME_NONNULL_END
