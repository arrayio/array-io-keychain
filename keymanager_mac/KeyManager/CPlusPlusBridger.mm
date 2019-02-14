//
//  CPlusPlusBridger.m
//  KeyManager
//
//  Created by Mikhail Lutskiy on 13/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

#import "CPlusPlusBridger.h"
#include <keychain_lib/keychain.hpp>

using namespace keychain_app;

@implementation CPlusPlusBridger

- (NSInteger) getKeyfilesCount {
    auto& keyfiles = keyfile_singleton::instance();
    auto distance = std::distance(keyfiles.begin(), keyfiles.end());
    return distance;
}

@end
