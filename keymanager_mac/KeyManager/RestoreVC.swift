//
//  RestoreVC.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 09/04/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

import Cocoa

@objc class RestoreVC: NSViewController {
    
    @objc var pathFile: String!
    @objc var needCancelSpecial = false
    @IBOutlet weak var seedField: NSSecureTextField!
    @IBOutlet weak var passField: NSSecureTextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
    @IBAction func restoreAction(_ sender: Any) {
        CPlusPlusBridger().restore(pathFile, seed: seedField.stringValue, pass: passField.stringValue)
        NotificationCenter.default.post(name: Notification.Name(rawValue: "reloadData"), object: nil)
        self.dismiss(self)
    }
    
    @IBAction func cancelAction(_ sender: Any) {
        if needCancelSpecial {
            AppleScriptManager.runScriptWithBody(Bundle.main.bundlePath.replacingOccurrences(of: "KeyManager.app", with: "keychain"))
            NSApplication.shared.terminate(self)
        }
        self.dismiss(self)
    }
}
