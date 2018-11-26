//
//  Consts.swift
//  Keychain Installer
//
//  Created by Mikhail Lutskiy on 22/11/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation

/// Constants
class Consts {
    
    struct Paths {
        static let OUT_LOG = "/out.txt"
        static let ERROR_LOG = "/log.txt"
        static let DEFAULT_FOLDER_NAME = "Keychain"
    }
    
    struct BuildArchivePath {
        static let NAME = "bin"
        static let EXTENSION = "tar.gz"
    }
    
    static let LABEL_JOB = "io.array.keychain"
    static let JOB_PLIST = LABEL_JOB + ".plist"
    
}
