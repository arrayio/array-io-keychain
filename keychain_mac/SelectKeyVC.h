//
//  SelectKeyVC.h
//  keychain
//
//  Created by Mikhail Lutskiy on 05/02/2019.
//

#import <Cocoa/Cocoa.h>
#include <keychain_lib/keychain.hpp>

NS_ASSUME_NONNULL_BEGIN

@interface SelectKeyVC : NSWindowController
    
//@property keychain_app::keyfile_singleton keyfiles;
    
- (instancetype)initWithFrame:(NSRect)frame;
- (void)runModal;


@end

NS_ASSUME_NONNULL_END
