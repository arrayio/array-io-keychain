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
    key.descriptionKey = [NSString stringWithUTF8String:index.description.c_str()];
    key.publicKey = [NSString stringWithUTF8String:index.public_key().hex().c_str()];
    auto seconds = index.creation_time.time_since_epoch().to_seconds();
    NSDate *createTime = [NSDate dateWithTimeIntervalSince1970:seconds];
    key.createTime = createTime;
    key.keychainVersion = @"0.15";
    key.cipherType = @"aes256";
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

- (void) reloadData {
    auto& keyfiles = keyfile_singleton::instance();
    keyfiles.keydata_load();
}

- (void) deleteKey:(NSString *)publicKey {
    auto& keyfiles = keyfile_singleton::instance();
    auto pkey = dev::Public([publicKey UTF8String]);
//    keyfiles.remove(pkey, std::bind(&remove_unlock))
//    keyfiles.remove(pkey, std::bind(&remove_unlock, my_unlock_functor));
}

- (NSMutableArray <Log *>*) getTransactionLogForPublicKey:(NSString *) publicKey {
    NSMutableArray <Log *>* logsArray = [NSMutableArray new];
    auto& keyfiles = keyfile_singleton::instance();
    NSLog(@"publicKey %@", publicKey);
    printf("%s", [publicKey UTF8String]);
    auto pkey = dev::Public([publicKey UTF8String]);
    try {
        auto& logs = keyfiles.get_logs(pkey);
        for (NSUInteger i = 0; i < std::distance(logs.begin(), logs.end()); i++) {
            auto& log = logs[i];
            Log *logItem = [[Log alloc] init];
            switch (log.blockchain_type) {
                case keychain_app::blockchain_te::array:
                    logItem.blockchainType = BlockchainTypeArray;
                    break;
                case keychain_app::blockchain_te::unknown:
                    logItem.blockchainType = BlockchainTypeUnknown;
                    break;
                case keychain_app::blockchain_te::bitshares:
                    logItem.blockchainType = BlockchainTypeBitshares;
                    break;
                case keychain_app::blockchain_te::ethereum:
                    logItem.blockchainType = BlockchainTypeEthereum;
                    break;
                case keychain_app::blockchain_te::bitcoin:
                    logItem.blockchainType = BlockchainTypeBitcoin;
                    break;
                case keychain_app::blockchain_te::rawhash:
                    logItem.blockchainType = BlockchainTypeRawhash;
                    break;
            }
    //        logItem.chainId = [NSString stringWithUTF8String:log.chainid.c_str()];
            auto seconds = log.sign_time.time_since_epoch().to_seconds();
            NSDate *signTime = [NSDate dateWithTimeIntervalSince1970:seconds];
            logItem.signTime = signTime;
            std::string transaction = keychain_app::to_hex(log.transaction.data(), log.transaction.size());
            logItem.transaction = [NSString stringWithUTF8String:transaction.c_str()];
            [logsArray addObject:logItem];
        }
    } catch (fc_light::exception& e) {
        NSLog(@"%@", [NSString stringWithUTF8String: e.to_detail_json_string().c_str()]);
    }
    return logsArray;
}

@end
