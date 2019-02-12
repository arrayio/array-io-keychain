//
//  KeyManager.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 24/12/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation
import ObjectMapper

struct KeysData {
    var fullKeyName: String
    var description: String
    var publicKey: String
    var location: String
    var cipherType: String
    var creationDate: String
    var keychainVersion: String
    var curveType: String
}

class KeyManager {
    
    static let shared = KeyManager()
    
    let fileManager = FileManager.default
    
    var keys = [KeysData]()
    
    func start () {
        keys.removeAll()
        loadFiledFromKeyData()
    }
    
    func loadFiledFromKeyData () {
        let keys = try! fileManager.contentsOfDirectory(atPath: "/var/keychain/key_data")
        print(keys)
        
        for key in keys {
            if key.fileExtension() == "json" {
                let fileAttr = try! fileManager.attributesOfItem(atPath: "/var/keychain/key_data/" + key)
                
                let keyModel = Mapper<KeyFileModel>().map(JSONString: try! String(contentsOfFile: "/var/keychain/key_data/" + key))
                let keyStruct = KeysData(fullKeyName: keyModel?.keyname ?? "",
                                         description: keyModel?.description ?? "",
                                         publicKey: keyModel?.keyInfo.publicKey ?? "",
                                         location: "/var/keychain/key_data/" + key,
                                         cipherType: keyModel?.keyInfo.privKeyData.cipherType ?? "",
                                         creationDate: String(describing: fileAttr[FileAttributeKey.creationDate] as! Date),
                                         keychainVersion: keyModel?.keychainVersion ?? "",
                                         curveType: keyModel?.keyInfo.curveType ?? "")
                self.keys.append(keyStruct)
            }
            print(self.keys)
            
        }
    }
    
}
