//
//  LocalStorage.swift
//  Keychain Installer
//
//  Created by Mikhail Lutskiy on 22/11/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation
/// Class for exchange data between ViewControllers
class LocalStorage {
    
    static let shared = LocalStorage()
    
    var pathForInstall = ""
    
    var isAutoStart = true
    
}
