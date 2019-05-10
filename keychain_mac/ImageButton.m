//
//  ImageButton.m
//  keychain
//
//  Created by Mikhail Lutskiy on 09/04/2019.
//

#import "ImageButton.h"

@implementation NSPopover (ImageButton)

- (id) initWithContent:(NSString *)content doesAnimate:(BOOL)animate {
    self = [self init];
    if (self) {
        self.behavior = NSPopoverBehaviorTransient;
        self.animates = animate;
        self.contentViewController = [NSViewController new];
        self.contentViewController.view = [[NSView alloc] initWithFrame:NSZeroRect];
        
        CGFloat popoverMargin = 20.0;
        
        NSTextField *textField = [[NSTextField alloc] initWithFrame:NSZeroRect];
        [textField setEditable:false];
        textField.stringValue = content;
        [textField setBordered:false];
        [textField setDrawsBackground:false];
        [textField sizeToFit];
        [textField setFrameOrigin:NSMakePoint(popoverMargin, popoverMargin)];
        
        [self.contentViewController.view addSubview:textField];
        
        CGSize viewSize = textField.frame.size;
        viewSize.width += popoverMargin * 2;
        viewSize.height += popoverMargin * 2;
        self.contentSize = viewSize;
    }
    return self;
}

@end
