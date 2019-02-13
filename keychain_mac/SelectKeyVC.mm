//
//  SelectKeyVC.m
//  keychain
//
//  Created by Mikhail Lutskiy on 05/02/2019.
//

#import "SelectKeyVC.h"
#import "MiddleAlignedTextFieldCell.h"

@interface SelectKeyVC () <NSTableViewDelegate, NSTableViewDataSource>

@end

@implementation SelectKeyVC
    
- (instancetype)initWithFrame:(NSRect)frame {
    NSWindowStyleMask windowMask = NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView | NSWindowStyleMaskClosable;
    NSWindow *window = [[NSWindow alloc] initWithContentRect:frame
                                                   styleMask:windowMask
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO];
    [window setTitle:@"KeyChain"];
    [window makeKeyAndOrderFront:self];
    [window orderFront:self];
    
    [NSNotificationCenter.defaultCenter addObserver:self
                                           selector:@selector(windowWillClose:)
                                               name:NSWindowWillCloseNotification
                                             object:nil];
    window.titlebarAppearsTransparent = YES;
    //window.titleVisibility = NSWindowTitleHidden;
    window.backgroundColor = [NSColor whiteColor];
    [window center];
    return [super initWithWindow:window];
}

- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}
    
- (void)windowWillClose:(NSNotification *)notification {
    [NSApp setActivationPolicy:NSApplicationActivationPolicyProhibited];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSApp stopModal];
    });
}
    
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
    {
        // NSString *aString = [NSString stringWithFormat:@"%@, Row %ld",[aTableColumn identifier],(long)rowIndex];
        NSString *aString;
//        aString = [[dataForBitcoin objectAtIndex:rowIndex] objectForKey:[aTableColumn identifier]];
        return aString;
    }
    
    // TableView Datasource method implementation
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
    {
        //we have only one table in the screen and thus we are not checking the row count based on the target table view
        //    long recordCount = [self.dataArray count];
        return 0;
    }
    
- (void)runModal {
    NSVisualEffectView *cover = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, self.window.frame.size.width, self.window.frame.size.height - 91)];
    cover.blendingMode = NSVisualEffectBlendingModeBehindWindow;
    cover.material = NSVisualEffectMaterialAppearanceBased;
    cover.state = NSVisualEffectStateFollowsWindowActiveState;
    [cover setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameVibrantLight]];
    [self.window.contentView addSubview:cover];

    
    [[NSApplication sharedApplication] runModalForWindow:self.window];
    
    [self.window setFrame:NSMakeRect(0, 0, 575, 500) display:true];
}
    
- (void) createTableView {
    NSScrollView *scrollView = [[NSScrollView alloc] initWithFrame:CGRectMake(22, 110, self.window.frame.size.width - 44, 150)];
    scrollView.backgroundColor = [NSColor clearColor];
    [scrollView setBorderType:NSBezelBorder];
    
    NSTableView *tableView = [[NSTableView alloc] initWithFrame:scrollView.bounds];
    tableView.rowSizeStyle = NSTableViewRowSizeStyleLarge;
    tableView.backgroundColor = [NSColor clearColor];
    tableView.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    
    NSTableColumn *tCol;
    
    tCol = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"key1"]];
    id cell = [tCol dataCell];
    [cell setFont: [NSFont systemFontOfSize:18]];
    [[tCol headerCell] setStringValue:@"To"];
    [tCol setWidth:self.window.frame.size.width - 149];
    tCol.editable = false;
    [[tCol dataCell] setVerticalCentering:YES];
    [tableView addTableColumn:tCol];
    
    tCol = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"key2"]];
    cell = [tCol dataCell];
    [cell setFont: [NSFont systemFontOfSize:18]];
    [[tCol headerCell] setStringValue:@"Amount"];
    [tCol setWidth:100];
    tCol.editable = false;
    [[tCol dataCell] setVerticalCentering:YES];
    [tableView addTableColumn:tCol];
    
    [tableView setUsesAlternatingRowBackgroundColors:YES];
    [tableView setGridStyleMask:NSTableViewSolidVerticalGridLineMask];
    [tableView setRowHeight:23.0];
    [tableView setDelegate:self];
    [tableView setDataSource:self];
    [tableView setSelectionHighlightStyle:NSTableViewSelectionHighlightStyleNone];
    [tableView setAutoresizesSubviews:YES];
    
    scrollView.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    [scrollView setHasVerticalScroller:YES];
    [scrollView setHasHorizontalScroller:NO];
    scrollView.horizontalScrollElasticity = NSScrollElasticityNone;
    [scrollView setAutoresizesSubviews:YES];
    [scrollView setAutoresizingMask:NSViewWidthSizable|NSViewHeightSizable];
    [scrollView setAutohidesScrollers:false];
    [scrollView setScrollerStyle:NSScrollerStyleLegacy];
    [scrollView setDocumentView:tableView];
    [self.window.contentView addSubview:scrollView];
}

@end
