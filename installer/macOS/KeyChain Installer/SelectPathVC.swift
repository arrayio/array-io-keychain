//
//  SelectPathVC.swift
//  Keychain Installer
//
//  Created by Mikhail Lutskiy on 22/11/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Cocoa

class SelectPathVC: NSViewController {
    
    @IBOutlet weak var pathControl: NSPathControl!
    @IBOutlet weak var autoLoadSelector: NSButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        if #available(OSX 10.12, *) {
            let pathUrl = FileManager.default.homeDirectoryForCurrentUser
            print("url")
            print(pathUrl.appendingPathComponent(Consts.Paths.DEFAULT_FOLDER_NAME, isDirectory: true))
            pathControl.url = pathUrl.appendingPathComponent(Consts.Paths.DEFAULT_FOLDER_NAME, isDirectory: true)
        } else {
            let pathUrl = NSHomeDirectory()
            pathControl.url = URL(fileURLWithPath: pathUrl + "/" + Consts.Paths.DEFAULT_FOLDER_NAME)
            // Fallback on earlier versions
        }
        LocalStorage.shared.pathForInstall = pathControl.url?.path ?? ""
    }
    
    @IBAction func selectPathAction(_ sender: Any) {
        let dialog = NSOpenPanel();
        
        dialog.title                   = "Choose directory for installation"
        dialog.showsResizeIndicator    = true
        dialog.showsHiddenFiles        = false
        dialog.canChooseDirectories    = true
        dialog.canCreateDirectories    = true
        dialog.canChooseFiles          = false
        dialog.allowsMultipleSelection = false;
        
        if (dialog.runModal() == NSApplication.ModalResponse.OK) {
            let result = dialog.url // Pathname of the file
            
            if (result != nil) {
                pathControl.url = result
                LocalStorage.shared.pathForInstall = result!.path
            }
        } else {
            // User clicked on "Cancel"
            return
        }
    }
    
    @IBAction func selectAutoLoadAction(_ sender: NSButton) {
        LocalStorage.shared.isAutoStart = self.autoLoadSelector.state == .on ? true : false
    }
}
