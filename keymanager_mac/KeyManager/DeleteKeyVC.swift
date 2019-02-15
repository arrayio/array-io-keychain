//
//  DeleteKeyVC.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 15/02/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

import Cocoa

class DeleteKeyVC: NSViewController {

    @IBOutlet weak var passwordTextField: NSSecureTextField!
    @IBOutlet weak var passwordLabel: NSTextField!
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
    }
    
    @IBAction func cancelAction(_ sender: Any) {
        self.dismiss(self)
    }
    
    @IBAction func deleteAction(_ sender: Any) {
    }
}
