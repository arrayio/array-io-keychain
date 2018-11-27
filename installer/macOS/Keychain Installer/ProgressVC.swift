//
//  ProgressVC.swift
//  Keychain Installer
//
//  Created by Mikhail Lutskiy on 22/11/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Cocoa
import AHLaunchCtl

class ProgressVC: NSViewController {
    
    let fileManager = FileManager.default
    @IBOutlet weak var progressSpinning: NSProgressIndicator!
    @IBOutlet weak var infoTextField: NSTextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.progressSpinning.startAnimation(true)
        self.progressSpinning.controlSize = .regular
        self.progressSpinning.isHidden = true
        self.infoTextField.isHidden = true
    }
    
    /// Function for create launchd job
    ///
    /// - Parameter dataPath: path of data on disk
    fileprivate func jobWorker(dataPath: String) {
        self.infoTextField.stringValue = "Searching old versions..."
        self.stopJobs()
        self.deleteFileAtPath(self.fileManager.homeDirectoryForCurrentUser.appendingPathComponent("Library/LaunchAgents/" + Consts.JOB_PLIST).path)
        self.addRpathToBinary(path: dataPath)
        if LocalStorage.shared.isAutoStart {
            self.infoTextField.stringValue = "Create autoload daemon..."
            let job = AHLaunchJob()
            job.label = Consts.LABEL_JOB
            job.programArguments = [
                dataPath + "/websocketd",
                "--port=8080",
                "--devconsole",
                dataPath+"/keychain"
            ]
            print(job.programArguments)
            job.standardOutPath = dataPath + Consts.Paths.OUT_LOG
            job.standardErrorPath = dataPath + Consts.Paths.ERROR_LOG
            job.debug = true
            job.runAtLoad = true
            job.keepAlive = true
            do {
                try AHLaunchCtl.sharedController()?.add(job, to: .globalLaunchDaemon)
            } catch {
                print(error.localizedDescription)
            }
        }
    }
    
    /// Function for show postinstallation alert and terminate application
    ///
    /// - Parameter text: text
    fileprivate func installText(_ text: String) {
        self.progressSpinning.stopAnimation(true)
        Alerts.dialogOKCancel(question: text, text: "")
        NSApplication.shared.terminate(self)
    }
    
    /// Function for unarchiving tar.gz archive and copy to disk
    func untar() {
        self.infoTextField.stringValue = "Unarchiving..."
        let dataPath = LocalStorage.shared.pathForInstall
        
        let path = Bundle.main.path(forResource: Consts.BuildArchivePath.NAME, ofType: Consts.BuildArchivePath.EXTENSION)
        print(dataPath)
        do {
            self.infoTextField.stringValue = "Create directory..."
            try fileManager.createDirectory(atPath: dataPath, withIntermediateDirectories: true, attributes: nil)
        } catch {
            print(error)
        }
        
        AppleScriptManager.runScriptWithBody("cd '" + dataPath + "' && tar -xzvf '" + path! + "'", success: {
            print("OK untar")
            print(dataPath)
            self.jobWorker(dataPath: dataPath)
            self.infoTextField.stringValue = "Installation complete!"
            self.installText("Install successfull")
        }) { (error) in
            print("ERROR: \(error)")
            self.installText(error.description)
        }
    }
    
    /// Function stop launchd job
    func stopJobs() {
        do {
            try AHLaunchCtl.sharedController()?.stop(Consts.LABEL_JOB, in: .userLaunchAgent)
        } catch {
            print(error.localizedDescription)
        }
        
        do {
            try AHLaunchCtl.sharedController()?.unload(Consts.LABEL_JOB, in: .userLaunchAgent)
        } catch {
            print(error.localizedDescription)
        }
        
        do {
            try AHLaunchCtl.sharedController()?.remove(Consts.LABEL_JOB, from: .userLaunchAgent)
        } catch {
            print(error.localizedDescription)
        }
    }
    
    /// Function delete file at path if exist
    ///
    /// - Parameter file: path of file
    func deleteFileAtPath (_ file: String) {
        if fileManager.fileExists(atPath: file) {
            do {
                try fileManager.removeItem(atPath: file)
            } catch let error as NSError {
                print(error)
            }
        }
    }
    
    @IBAction func installAction(_ sender: Any) {
        self.progressSpinning.isHidden = false
        self.infoTextField.isHidden = false
        self.untar()
    }
    
    /// Function fix library rpath for Binary
    ///
    /// - Parameter path: path to project
    func addRpathToBinary(path: String) {
        //        AppleScriptManager.runScriptWithBody("install_name_tool -add_rpath '" + path + "/secp256k1custom' " + path + "/witness_node/witness_node", failure: {
        //            error in
        //            print(error)
        //        })
        //
        //        AppleScriptManager.runScriptWithBody("install_name_tool -add_rpath '" + path + "/secp256k1custom' " + path + "/cli_wallet/cli_wallet", failure: {
        //            error in
        //            print(error)
        //        })
    }
    
}
