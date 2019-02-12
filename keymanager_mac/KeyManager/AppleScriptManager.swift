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
    static func runScriptWithBody(_ body: String, isAdminRequired: Bool = false, success: @escaping()  -> Void = {}, failure: @escaping(_ error: NSDictionary) -> Void = {error in }) {
//        let script = NSAppleScript(source: "do shell script \"" + body + "\"")
//        var errorInfo: NSDictionary?
//        script?.executeAndReturnError(&errorInfo)
//        if let error = errorInfo {
//            failure(error)
//        } else {
//            success()
//        }
        runScriptWithBodyWithReturnString(body, isAdminRequired: isAdminRequired, success: { _ in
            success()
        }) { (error) in
            failure(error)
        }
    }
    
    static func runScriptWithBodyWithReturnString(_ body: String, isAdminRequired: Bool = false, success: @escaping(_ result: String)  -> Void = {_ in }, failure: @escaping(_ error: NSDictionary) -> Void = {error in }) {
        let script = NSAppleScript(source: "do shell script \"" + body + "\"" + (isAdminRequired ? " with administrator privileges" : ""))
        var errorInfo: NSDictionary?
        let result = script?.executeAndReturnError(&errorInfo)
        if let error = errorInfo {
            failure(error)
        } else {
            success(result?.stringValue ?? "")
        }
    }
    
}
