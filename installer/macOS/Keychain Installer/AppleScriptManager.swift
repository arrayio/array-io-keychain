//
//  AppleScriptManager.swift
//  Keychain Installer
//
//  Created by Mikhail Lutskiy on 22/11/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation

class AppleScriptManager {
    
    /// Function for call apple script (execute shell commands)
    ///
    /// - Returns: return success or failure block
    static func runScriptWithBody(_ body: String, success: @escaping()  -> Void = {}, failure: @escaping(_ error: NSDictionary) -> Void = {error in }) {
        let script = NSAppleScript(source: "do shell script \"" + body + "\"")
        var errorInfo: NSDictionary?
        script?.executeAndReturnError(&errorInfo)
        if let error = errorInfo {
            failure(error)
        } else {
            success()
        }
    }
    
}
