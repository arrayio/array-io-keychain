//
//  EntropyCreatingVC.m
//  keychain
//
//  Created by Mikhail Lutskiy on 07/03/2019.
//

#import "EntropyCreatingVC.h"
#import "LogoView.h"
#import "HexToRgbColor.h"
#import "SYFlatButton.h"

@interface EntropyCreatingVC () {
    NSView *firstSceneView;
    NSView *secondSceneView;
    NSTimer *timerSecond;
}

@end

@implementation EntropyCreatingVC

- (instancetype)initWithFrame:(NSRect)frame {
    NSWindowStyleMask windowMask = NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView;
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

- (void)dealloc {
    [NSNotificationCenter.defaultCenter removeObserver:self];
}

- (void)windowWillClose:(NSNotification *)notification {
    [NSApp setActivationPolicy:NSApplicationActivationPolicyProhibited];
    dispatch_async(dispatch_get_main_queue(), ^{
        [NSApp stopModal];
    });
}

- (void)runModal {
    [LogoView setLogoViewForWindow:self.window withTitle:@"Protecting your keys with entropy"];
    [[self getFirstScene] setHidden:false];
//    [self setupLogoView];
//    [self createTableView];
//    [self setupCancelButton];
//    [self setupOkButton];
    [[NSApplication sharedApplication] runModalForWindow:self.window];
    
    [self.window setFrame:NSMakeRect(0, 0, 575, 500) display:true];
}

- (NSView *) getFirstScene {
    if (!firstSceneView) {
        firstSceneView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, self.window.frame.size.width, 400)];
        [self.window.contentView addSubview:firstSceneView];
        
        NSTextField *label = [NSTextField labelWithString:@"We use entropy as the first step to protecting your keys. Please make as much noise as you possibly can and move your mouse vigorously."];
        label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
        label.font = [NSFont systemFontOfSize:16];
        label.frame = NSMakeRect(64, self.window.frame.size.height - 200, self.window.frame.size.width - 128, 54);
        label.lineBreakMode = NSLineBreakByWordWrapping;
        label.alignment = NSTextAlignmentCenter;
        [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [firstSceneView addSubview:label];
        
        NSTextField *label2 = [NSTextField labelWithString:@"We hope you have fun while we collect entropy data to confuse anyone who might want to access your keys."];
        label2.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        label2.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
        label2.font = [NSFont systemFontOfSize:16];
        label2.frame = NSMakeRect(64, 30, self.window.frame.size.width - 128, 54);
        label2.lineBreakMode = NSLineBreakByWordWrapping;
        label2.alignment = NSTextAlignmentCenter;
        [label2 setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [firstSceneView addSubview:label2];
        
        NSTextField *label3 = [NSTextField labelWithString:@"Entropy generation..."];
        label3.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        label3.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
        label3.font = [NSFont systemFontOfSize:14];
        label3.frame = NSMakeRect(64, 180, self.window.frame.size.width - 128, 54);
        label3.lineBreakMode = NSLineBreakByWordWrapping;
        label3.alignment = NSTextAlignmentCenter;
        [label3 setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [firstSceneView addSubview:label3];
        
        NSProgressIndicator *indicator = [[NSProgressIndicator alloc] initWithFrame:NSMakeRect(64, 240, self.window.frame.size.width - 128, 20)];
        indicator.style = NSProgressIndicatorStyleBar;
        indicator.minValue = 0;
        indicator.indeterminate = false;
        indicator.maxValue = 100.0;
        indicator.doubleValue = 0.0;
        NSLog(@"indicator");
        NSTimer *timer = [NSTimer scheduledTimerWithTimeInterval:0.5 repeats:true block:^(NSTimer * _Nonnull timer) {
            NSLog(@"timer");
            indicator.doubleValue += 10.0;
            if (indicator.doubleValue == 100) {
                [timer invalidate];
                NSLog(@"1234");
                [[self getFirstScene] setHidden:true];
                [[self getSecondView] setHidden:false];
            }
        }];
        [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];

        [firstSceneView addSubview:indicator];
    }
    return firstSceneView;
}

- (NSView *) getSecondView {
    if (!secondSceneView) {
        secondSceneView = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, self.window.frame.size.width, 400)];
        [self.window.contentView addSubview:secondSceneView];
        
        NSTextField *label = [NSTextField labelWithString:@"Congratulations!\nYour keys are protected with entropy.\n\nMemorize the seed displayed below. You will need it to restore your keys!\n\nThis message will self-destruct in 3 minutes and the seed will not be shown again for security reasons."];
        label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
        label.font = [NSFont systemFontOfSize:16];
        label.frame = NSMakeRect(64, self.window.frame.size.height - 350, self.window.frame.size.width - 128, 200);
        label.lineBreakMode = NSLineBreakByWordWrapping;
        label.alignment = NSTextAlignmentCenter;
        [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [secondSceneView addSubview:label];
        
        NSTextField *label2 = [NSTextField labelWithString:@"123456 123456 123456 123456 123456 123456\n123456 123456 123456 123456 123456 123456"];
        label2.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        label2.textColor = [NSColor redColor];
        label2.font = [NSFont systemFontOfSize:27];
        label2.frame = NSMakeRect(32, 30, self.window.frame.size.width - 64, 100);
        label2.lineBreakMode = NSLineBreakByWordWrapping;
        label2.alignment = NSTextAlignmentCenter;
        [label2 setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [secondSceneView addSubview:label2];
        
        NSTextField *label3 = [NSTextField labelWithString:@"180 seconds"];
        label3.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
        label3.textColor = [NSColor blackColor];
        label3.font = [NSFont systemFontOfSize:20];
        label3.frame = NSMakeRect(64, 160, self.window.frame.size.width - 128, 54);
        label3.lineBreakMode = NSLineBreakByWordWrapping;
        label3.alignment = NSTextAlignmentCenter;
        [label3 setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
        [secondSceneView addSubview:label3];
        
        __block int timerSec = 180;
        
        timerSecond = [NSTimer scheduledTimerWithTimeInterval:1 repeats:true block:^(NSTimer * _Nonnull timer) {
            NSLog(@"timer");
            timerSec -= 1;
            label3.stringValue = [NSString stringWithFormat:@"%d seconds", timerSec];
            
            if (timerSec == 0) {
                [timer invalidate];
                NSLog(@"1234");
                [[self getSecondView] setHidden:true];
            }
        }];
        [[NSRunLoop currentRunLoop] addTimer:timerSecond forMode:NSRunLoopCommonModes];
        
        SYFlatButton *button = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 125, 20, 100, 35)];
        button.target = self;
        button.action = @selector(clickButton);
        button.title = @"CLOSE";
        button.titleNormalColor = [NSColor whiteColor];
        button.momentary = YES;
        button.cornerRadius = 4.0;
        button.backgroundNormalColor = [HexToRgbColor colorWithHexColorString:@"4686FF"];
        [secondSceneView addSubview:button];
        
        SYFlatButton *buttonPauseResume = [[SYFlatButton alloc] initWithFrame:NSMakeRect(self.window.frame.size.width - 245, 20, 100, 35)];
        buttonPauseResume.target = self;
        buttonPauseResume.action = @selector(pauseResumeButton);
        buttonPauseResume.title = @"PAUSE";
        buttonPauseResume.backgroundNormalColor = [NSColor whiteColor];
        buttonPauseResume.titleNormalColor = [HexToRgbColor colorWithHexColorString:@"939497"];
        buttonPauseResume.cornerRadius = 4;
        buttonPauseResume.momentary = YES;
        [secondSceneView addSubview:buttonPauseResume];
        
    }
    return secondSceneView;
}

- (void) pauseResumeButton {
    [timerSecond invalidate];
}

@end
