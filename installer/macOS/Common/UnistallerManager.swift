//
//  UninstallerManager.swift
//  KeyChain Installer
//
//  Created by Mikhail Lutskiy on 19/12/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation

enum UninstallerManagerSteps {
    case autoRunProcess
    case webSocketBinary
    case keyChainBinary
    case keyChainResources
    case keyChainExamples
    case keyChainLogs
    case keyChainKeys
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
    
    func launch (){
        disableAutoRunProcess()
        deleteWebSocketBinary()
        deleteKeyChainBinary()
        deleteKeyChainResources()
        deleteKeyChainExamples()
        deleteKeyChainLogs()
        deleteKeyChainKeys()
        delegate?.successfullyUninstall()
    }
    
    func disableAutoRunProcess () {
        
    }
    
    func deleteWebSocketBinary () {
        
    }
    
    func deleteKeyChainBinary () {
        
    }
    
    func deleteKeyChainResources () {
        
    }
    
    func deleteKeyChainExamples () {
        
    }
    
    func deleteKeyChainLogs () {
        
    }
    
    func deleteKeyChainKeys () {
        if isDeleteKeys {
            
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
        }
    }
    
}
