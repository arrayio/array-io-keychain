//
//  TableView.m
//  keychain
//
//  Created by Mikhail Lutskiy on 13/02/2019.
//

#import "TableView.h"

@implementation TableView

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    
    // Drawing code here.
}

- (void)mouseDown:(NSEvent *)theEvent {
    
    NSPoint globalLocation = [theEvent locationInWindow];
    NSPoint localLocation = [self convertPoint:globalLocation fromView:nil];
    NSInteger clickedRow = [self rowAtPoint:localLocation];
    
    if(clickedRow != -1) {
        [super mouseDown:theEvent];
    }
}

@end
