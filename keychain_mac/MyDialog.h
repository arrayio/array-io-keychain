//
//  MyDialog.h
//  keychain
//
//  Created by Mikhail Lutskiy on 29/10/2018.
//

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface MyDialog : NSWindowController

@property (nonatomic, strong) NSString *jsonString;
@property (readwrite) BOOL isSignTransaction;
@property (nonatomic, strong) NSString *currentPath;

- (instancetype)initWithFrame:(NSRect)frame;
- (void)runModal;
@end

NS_ASSUME_NONNULL_END
