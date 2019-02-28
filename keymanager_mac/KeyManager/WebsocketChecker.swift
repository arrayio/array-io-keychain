//
//  WebsocketChecker.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 26/12/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation

class WebsocketChecker {
    
    static func checkForRunningWebsocketd (success: @escaping(_ isRunning: Bool) -> Void) {
        AppleScriptManager.runScriptWithBodyWithReturnString("ps -ax -o command | grep websocket | grep -v grep", success: { (result) in
            if result != "" {
                success(true)
            } else {
                success(false)
            }
        }) { (error) in
            print(error)
            success(false)
        }
    }
    
}
