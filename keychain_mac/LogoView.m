//
//  LogoView.m
//  keychain
//
//  Created by Mikhail Lutskiy on 07/03/2019.
//

#import "LogoView.h"
#import "FileManager.h"
#import "HexToRgbColor.h"

@implementation LogoView

+ (void)setLogoViewForWindow:(NSWindow *)window {
    [self setLogoViewForWindow:window withTitle:@""];
}

+ (void)setLogoViewForWindow:(NSWindow *)window withTitle:(NSString *) title {
    NSTextField *label = [NSTextField labelWithString:title];
    label.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    label.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    label.font = [NSFont systemFontOfSize:14];
    label.frame = NSMakeRect(440, window.frame.size.height - 98, window.frame.size.width - 231, 54);
    label.lineBreakMode = NSLineBreakByWordWrapping;
    [label setContentCompressionResistancePriority:250 forOrientation:NSLayoutConstraintOrientationHorizontal];
    
    [window.contentView addSubview:label];
    
    NSVisualEffectView *cover = [[NSVisualEffectView alloc] initWithFrame:NSMakeRect(0, 0, window.frame.size.width, window.frame.size.height - 91)];
    cover.blendingMode = NSVisualEffectBlendingModeBehindWindow;
    cover.material = NSVisualEffectMaterialAppearanceBased;
    cover.state = NSVisualEffectStateFollowsWindowActiveState;
    [cover setAppearance:[NSAppearance appearanceNamed:NSAppearanceNameVibrantLight]];
    [window.contentView addSubview:cover];
    
    NSImageView *imageView = [[NSImageView alloc] initWithFrame:NSMakeRect(22, window.frame.size.height - 81, 64, 54)];
    NSImage *image = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithFormat:@"%@/%@", FileManager.getWorkDirectoryPath, @"resources/logo.png"]];
    imageView.image = image;
    [window.contentView addSubview:imageView];
    
    NSTextField *labelLogo = [NSTextField labelWithString:@"KeyChain"];
    labelLogo.appearance = [NSAppearance appearanceNamed:NSAppearanceNameAqua];
    labelLogo.textColor = [HexToRgbColor colorWithHexColorString:@"4f4e4e"];
    labelLogo.font = [NSFont systemFontOfSize:18];
    labelLogo.frame = NSMakeRect(90, window.frame.size.height - 96, 100, 54);
    
    [window.contentView addSubview:labelLogo];
}

@end
