//
//  UninstallVC.swift
//  Keychain Uninstaller
//
//  Created by Mikhail Lutskiy on 24/12/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Cocoa

class UninstallVC: NSViewController, UninstallerManagerDelegate {

    @IBOutlet weak var spinningIndicator: NSProgressIndicator!
    @IBOutlet weak var isSave: NSButton!
    @IBOutlet weak var detailText: NSTextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        spinningIndicator.isHidden = true
        detailText.isHidden = true
    }
    
    @IBAction func uninstallAction(_ sender: Any) {
        spinningIndicator.isHidden = false
        detailText.isHidden = false
        detailText.stringValue = "Deleting..."
        spinningIndicator.startAnimation(self)
        
        UninstallerManager.shared.isDeleteKeys = self.isSave.state == .on ? false : true
        UninstallerManager.shared.delegate = self
        UninstallerManager.shared.launch()
    }
    
    @IBAction func saveKeysAction(_ sender: Any) {
        if self.isSave.state != .on {
            Alerts.dialogOKCancel(question: "Keys will be permanently deleted.", text: "Attention")
        }
    }
    
    func endOfStep(_ step: UninstallerManagerSteps) {
        
    }
    
    func successfullyUninstall() {
        Alerts.dialogOKCancel(question: "Uninstallation complete!", text: "")
        NSApplication.shared.terminate(self)
    }
    
    func failOfStep(_ step: UninstallerManagerSteps) {
        
    }
}
