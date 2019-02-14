//
//  CreateKeyVC.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 14/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

import Cocoa

class CreateKeyVC: NSViewController {

    @IBOutlet weak var nameTextField: NSTextField!
    @IBOutlet weak var descriptionTextField: NSTextField!
    @IBOutlet weak var cipherPopUpButton: NSPopUpButton!
    @IBOutlet weak var passwordTextField: NSSecureTextField!
    @IBOutlet weak var rePasswordTextField: NSSecureTextField!
    @IBOutlet weak var passwordView: NSView!
    
    var isEncrypted = true
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
    @IBAction func cancelAction(_ sender: Any) {
        self.dismiss(self)
    }
    
    @IBAction func createAction(_ sender: Any) {
        if nameTextField.stringValue != "" {
            if isEncrypted {
                if passwordTextField.stringValue == rePasswordTextField.stringValue && passwordTextField.stringValue != "" {
                    CPlusPlusBridger().createKey(withName: nameTextField.stringValue, description: descriptionTextField.stringValue, encrypted: isEncrypted, password: passwordTextField.stringValue, cipher: "")
                    self.dismiss(self)
                } else {
                    let alert = NSAlert()
                    alert.icon = NSImage()
                    alert.messageText = "Error"
                    alert.informativeText = "Passphrases must match and not be empty"
                    alert.addButton(withTitle: "OK")
                    alert.beginSheetModal(for: self.view.window!, completionHandler: nil)
                }
            }
        } else {
            let alert = NSAlert()
            alert.icon = NSImage()
            alert.messageText = "Error"
            alert.informativeText = "Keyname must not be empty"
            alert.addButton(withTitle: "OK")
            alert.beginSheetModal(for: self.view.window!, completionHandler: nil)
        }
    }
    
    @IBAction func clickToEncryptedCheckbox(_ sender: NSButton) {
        if sender.state == .on {
            isEncrypted = true
            passwordView.isHidden = false
        } else if sender.state == .off {
            isEncrypted = false
            passwordView.isHidden = true
        }
    }
}
