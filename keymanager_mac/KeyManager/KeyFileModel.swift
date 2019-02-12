//
//  KeyFileModel.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 24/12/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation
import ObjectMapper

class KeyFileModel: Mappable {
    
    var filetype: String!
    var keyname: String!
    var description: String!
    var keychainVersion: String!
    var keyInfo: KeyInfoModel!
    
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        filetype <- map["filetype"]
        keyname <- map["keyname"]
        description <- map["description"]
        keychainVersion <- map["keychain_version"]
        keyInfo <- map["keyinfo"]
    }
}

class KeyInfoModel: Mappable {
    
    var format: String!
    var encrypted: Bool!
    var curveType: String!
    var privKeyData: PrivKeyDataModel!
    var publicKey: String!
    
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        format <- map["format"]
        encrypted <- map["encrypted"]
        curveType <- map["curve_type"]
        privKeyData <- map["priv_key_data"]
        publicKey <- map["public_key"]
    }
}

class PrivKeyDataModel: Mappable {
    
    var cipherType: String!
    var iv: String!
    var encData: String!
    
    required init?(map: Map) {
        
    }
    
    func mapping(map: Map) {
        cipherType <- map["cipher_type"]
        iv <- map["iv"]
        encData <- map["enc_data"]
    }
}
