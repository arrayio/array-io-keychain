//
//  UninstallerManager.swift
//  KeyChain Installer
//
//  Created by Mikhail Lutskiy on 19/12/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation
import AHLaunchCtl

enum UninstallerManagerSteps {
    case autoRunProcess
    case webSocketBinary
    case keyChainBinary
    case keyChainResources
    case keyChainExamples
    case keyChainLogs
    case keyChainKeys
    case keyChainDir
}

protocol UninstallerManagerDelegate {
    func endOfStep(_ step: UninstallerManagerSteps)
    func successfullyUninstall()
    func failOfStep(_ step: UninstallerManagerSteps)
}

extension UninstallerManagerDelegate {
    func endOfStep(_ step: UninstallerManagerSteps) {}
    func successfullyUninstall() {}
    func failOfStep(_ step: UninstallerManagerSteps) {}
}

class UninstallerManager {
    
    static let shared = UninstallerManager()
    
    var delegate: UninstallerManagerDelegate?
    
    let fileManager = FileManager.default
    
    var isDeleteKeys = false
    
    let ahLaunchCtl = AHLaunchCtl()
    
    func launch () {
        ahLaunchCtl.authorize()
        print(fileManager.currentDirectoryPath)
        predictKeychainPath(success: { (path) in
            self.deleteForKeyChainPath(path)
        }) {
            self.deleteForKeyChainPath(self.fileManager.currentDirectoryPath)
        }
        disableAutoRunProcess()
        deleteKeyChainKeys()
        delegate?.successfullyUninstall()
    }
    
    func deleteForKeyChainPath(_ path: String) {
        deleteWebSocketBinary(path: path)
        deleteKeyChainBinary(path: path)
        deleteKeyChainResources(path: path)
        deleteKeyChainExamples(path: path)
        deleteKeyChainLogs(path: path)
        deleteFile(path, step: .keyChainDir)
    }
    
    func predictKeychainPath (success: @escaping(_ path: String) -> Void, fail:@escaping() -> Void) {
        AppleScriptManager.runScriptWithBodyWithReturnString("ps -ax -o command | grep websocket | grep -v grep", success: { (result) in
            let subcmd = result.components(separatedBy: " ")
            let path = subcmd.first?.replacingOccurrences(of: "/websocketd", with: "") ?? ""
            if path != "" {
                success(path)
            } else {
                fail()
            }
            print(path)
        }) { (error) in
            print(error)
            fail()
        }
    }
    
    func disableAutoRunProcess () {
        do {
            try ahLaunchCtl.stop(Consts.LABEL_JOB, in: .userLaunchAgent)
        } catch {
            print(error.localizedDescription)
        }
        do {
            try ahLaunchCtl.unload(Consts.LABEL_JOB, in: .userLaunchAgent)
        } catch {
            print(error.localizedDescription)
        }
        do {
            try ahLaunchCtl.remove(Consts.LABEL_JOB, from: .userLaunchAgent)
        } catch {
            print(error.localizedDescription)
        }
    }
    
    func deleteWebSocketBinary (path: String) {
        deleteFile(path + "/websocketd", step: .webSocketBinary)
    }
    
    func deleteKeyChainBinary (path: String) {
        deleteFile(path + "/keychain", step: .webSocketBinary)
    }
    
    func deleteKeyChainResources (path: String) {
        deleteFile(path + "/resources", step: .webSocketBinary)
    }
    
    func deleteKeyChainExamples (path: String) {
        deleteFile(path + "/examples", step: .webSocketBinary)
    }
    
    func deleteKeyChainLogs (path: String) {
        deleteFile(path + "/log.txt", step: .webSocketBinary)
        deleteFile(path + "/out.txt", step: .webSocketBinary)
    }
    
    func deleteKeyChainKeys () {
        if isDeleteKeys {
            AppleScriptManager.runScriptWithBody("rm -rf /var/keychain", isAdminRequired: true)
        }
    }
    
    func deleteFile (_ file: String, step: UninstallerManagerSteps) {
        if fileManager.fileExists(atPath: file) {
            do {
                try fileManager.removeItem(atPath: file)
                delegate?.endOfStep(step)
            } catch let error as NSError {
                print(error)
                delegate?.failOfStep(step)
            }
        } else {
            delegate?.failOfStep(step)
        }
    }
    
}
