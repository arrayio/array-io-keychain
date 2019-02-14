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

- (Key *) getKeyById:(NSInteger) indexRow {
    auto& keyfiles = keyfile_singleton::instance();
    auto& index = keyfiles[indexRow];
    Key *key = [[Key alloc] init];
    key.name = [NSString stringWithUTF8String:index.keyname.c_str()];
    return key;
}

- (void) createKeyWithName:(NSString* )keyname description:(NSString*)keyDescription encrypted:(BOOL)encrypted password:(NSString *) password cipher:(NSString *) cipher {
    
    auto pass = [password UTF8String];
//    typedef typename decltype(pass)::print_type print;
    
    auto& keyfiles = keyfile_singleton::instance();
    
    keyfiles.create(std::bind(&create_new_keyfile, [keyname UTF8String], [keyDescription UTF8String], encrypted, keyfile_format::cipher_etype::aes256, keyfile_format::curve_etype::secp256k1, [&pass](const std::string& keyname) {
        std::vector<char> result;
        std::copy(pass, pass + strlen(pass), std::back_inserter(result));
        return result;
    }));
}


@end
