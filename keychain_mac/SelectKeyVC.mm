//
//  SelectKeyVC.m
//  keychain
//
//  Created by Mikhail Lutskiy on 05/02/2019.
//

#import "SelectKeyVC.h"
#import "MiddleAlignedTextFieldCell.h"
#import "FileManager.h"
#import "HexToRgbColor.h"
#import "SYFlatButton.h"
#import "PassSyncStore.h"
#import "TableView.h"
#import "LogoView.h"

using namespace keychain_app;

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
//    window.backgroundColor = [NSColor whiteColor];
    [window center];
    return [super initWithWindow:window];
}

- (void)windowDidLoad {
    [super windowDidLoad];
    
    // Implement this method to handle any initialization after your window controller's window has been loaded from its nib file.
}

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
}
    
- (void)windowWillClose:(NSNotification *)notification {
    [NSApp setActivationPolicy:NSApplicationActivationPolicyProhibited];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSApp stopModal];
    });
}
    
- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex {
    auto& keyfiles = keyfile_singleton::instance();
    
    auto& index = keyfiles.random_access_index();
    auto it = index.begin() + rowIndex;
    auto key = *(it);
    NSString *aString;

    if ([[aTableColumn identifier] isEqualToString:@"key1"]) {
        aString = [NSString stringWithUTF8String:key.keyname.c_str()];
    } else if ([[aTableColumn identifier] isEqualToString:@"key2"]) {
        aString = [self getSubstrStr:[NSString stringWithUTF8String:key.public_key().hex().c_str()]];
    } else if ([[aTableColumn identifier] isEqualToString:@"key3"]) {
        auto seconds = key.creation_time.time_since_epoch().to_seconds();
        NSDate *createTime = [NSDate dateWithTimeIntervalSince1970:seconds];
        NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
        [formatter setDateFormat:@"MMM dd, YYYY, HH:mm:ss"];
        NSString *stringFromDate = [formatter stringFromDate:createTime];
        aString = stringFromDate;
    }
    return aString;
}

- (NSString *) getSubstrStr:(NSString*) str {
    NSString *substring = [str substringToIndex:32];
    NSString *substringLast = [str substringFromIndex:str.length-8];
    return [NSString stringWithFormat:@"%@<...>%@", substring, substringLast];
}
    
    // TableView Datasource method implementation
- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView
    {
        auto& keyfiles = keyfile_singleton::instance();
        auto distance = std::distance(keyfiles.begin(), keyfiles.end());
        return distance;
    }
    
- (void)runModal {
    [LogoView setLogoViewForWindow:self.window withTitle:@"Select key"];
    [self createTableView];
    [self setupCancelButton];
    [self setupOkButton];
    [[NSApplication sharedApplication] runModalForWindow:self.window];
    
    [self.window setFrame:NSMakeRect(0, 0, 575, 500) display:true];
}
    
- (void) createTableView {
    NSScrollView *scrollView = [[NSScrollView alloc] initWithFrame:CGRectMake(22, 80, self.window.frame.size.width - 44, 350)];
    scrollView.backgroundColor = [NSColor clearColor];
    [scrollView setBorderType:NSBezelBorder];
    
    TableView *tableView = [[TableView alloc] initWithFrame:scrollView.bounds];
    tableView.rowSizeStyle = NSTableViewRowSizeStyleLarge;
    tableView.backgroundColor = [NSColor clearColor];
//    tableView.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    
    NSTableColumn *tCol;
    
    tCol = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"key1"]];
    id cell = [tCol dataCell];
    [cell setFont: [NSFont systemFontOfSize:18]];
    [[tCol headerCell] setStringValue:@"Keyname"];
    [tCol setWidth:100];
    tCol.editable = false;
    [[tCol dataCell] setVerticalCentering:YES];
    [tableView addTableColumn:tCol];

    tCol = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"key2"]];
    cell = [tCol dataCell];
    [cell setFont: [NSFont systemFontOfSize:18]];
    [[tCol headerCell] setStringValue:@"Public key"];
    tCol.editable = false;
    [tCol setWidth:480];
    [[tCol dataCell] setVerticalCentering:YES];
    [tableView addTableColumn:tCol];
    
    tCol = [[NSTableColumn alloc] initWithIdentifier:[NSString stringWithFormat:@"key3"]];
    cell = [tCol dataCell];
    [cell setFont: [NSFont systemFontOfSize:18]];
    [[tCol headerCell] setStringValue:@"Date"];
    [tCol setWidth:250];
    tCol.editable = false;
    [[tCol dataCell] setVerticalCentering:YES];
    [tableView addTableColumn:tCol];
    
    [tableView setUsesAlternatingRowBackgroundColors:YES];
    [tableView setGridStyleMask:NSTableViewSolidVerticalGridLineMask];
    [tableView setRowHeight:23.0];
    [tableView setDelegate:self];
    [tableView setDataSource:self];
    tableView.selectionHighlightStyle = NSTableViewSelectionHighlightStyleRegular;
//    [tableView setSelectionHighlightStyle:NSTableViewSelectionHighlightStyleNone];
    [tableView setAutoresizesSubviews:YES];

//    scrollView.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
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

- (void) setupCancelButton {
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 245, 20, 100, 35)];
    button.target = self;
    button.action = @selector(clickCloseButton);
    button.title = @"CANCEL";
    button.backgroundNormalColor = [NSColor whiteColor];
    button.titleNormalColor = [HexToRgbColor colorWithHexColorString:@"939497"];
    button.cornerRadius = 4;
    button.momentary = YES;
    [self.window.contentView addSubview:button];
}

- (void) setupOkButton {
    SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 125, 20, 100, 35)];
    button.target = self;
    button.action = @selector(clickButton);
    button.title = @"OK";
    button.titleNormalColor = [NSColor whiteColor];
    button.momentary = YES;
    button.cornerRadius = 4.0;
    button.backgroundNormalColor = [HexToRgbColor colorWithHexColorString:@"4686FF"];
    [self.window.contentView addSubview:button];
}

- (void) clickCloseButton {
    [[PassSyncStore sharedInstance] setButtonClickType:ButtonClickTypeCancel];
    [self.window close];
}

- (void) clickButton {
    [[PassSyncStore sharedInstance] setButtonClickType:ButtonClickTypeOK];
    [self.window close];
}

- (void)tableViewSelectionDidChange:(NSNotification *)notification {
    NSLog(@"selected %@", [notification object]);

    auto& keyfiles = keyfile_singleton::instance();
    auto& index = keyfiles.random_access_index();
    auto it = index.begin() + [[notification object] selectedRow];
    auto key = *(it);
    [[PassSyncStore sharedInstance] setPublic_key:key.public_key()];
}

@end
