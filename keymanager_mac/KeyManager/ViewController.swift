//
//  ViewController.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 24/12/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Cocoa

class ViewController: NSViewController, NSTableViewDelegate, NSTableViewDataSource {

    @IBOutlet weak var tableView: NSTableView!
    @IBOutlet weak var websocketStatusLabel: NSTextField!
    @IBOutlet weak var creationDate: NSTextFieldCell!
    @IBOutlet weak var keychainVersion: NSTextFieldCell!
    @IBOutlet weak var cipherType: NSTextFieldCell!
    @IBOutlet weak var location: NSTextFieldCell!
    @IBOutlet weak var descriptionKey: NSTextFieldCell!
    @IBOutlet weak var publicKey: NSTextFieldCell!
    @IBOutlet weak var detailsView: NSView!
//    @IBOutlet weak var titleView: NSView!
    
    fileprivate enum CellIdentifiers {
        static let LocationCell = "locationId"
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        KeyManager.shared.start()
        tableView.reloadData()
        self.view.layer?.backgroundColor = NSColor(red: 242.0/255.0, green: 243.0/255.0, blue: 247.0/255.0, alpha: 1).cgColor
        self.detailsView.wantsLayer = true
        self.detailsView.layer?.backgroundColor = NSColor(red: 213.0/255.0, green: 220.0/255.0, blue: 230.0/255.0, alpha: 1).cgColor
        
//        titleView.wantsLayer = true
//        titleView.layer?.backgroundColor = NSColor.white.cgColor
        
        websocketStatus()
        Timer.scheduledTimer(timeInterval: 5, target: self, selector: #selector(websocketStatus), userInfo: nil, repeats: true)
        // Do any additional setup after loading the view.
    }
    
    func updateTableView () {
        
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        
        var text: String = ""
        var cellIdentifier: String = ""
        
        let item = KeyManager.shared.keys[row]
        
        if tableColumn == tableView.tableColumns[0] {
            text = item.fullKeyName
            cellIdentifier = CellIdentifiers.LocationCell
        }
        
        // 3
        if let cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: nil) as? NSTableCellView {
            cell.textField?.stringValue = text
            return cell
        }
        return nil
        
    }
        
    func numberOfRows(in tableView: NSTableView) -> Int {
        return CPlusPlusBridger().getKeyfilesCount()
//        return KeyManager.shared.keys.count
    }
    
    @objc func websocketStatus() {
        WebsocketChecker.checkForRunningWebsocketd { (isRunning) in
            self.websocketStatusLabel.stringValue = isRunning ? "Running" : "Not started"
        }
    }
    
    func tableView(_ tableView: NSTableView, didClick tableColumn: NSTableColumn) {
        print("click")
    }
    
    func tableView(_ tableView: NSTableView, rowViewForRow row: Int) -> NSTableRowView? {
        return TableRowView()
    }

    func tableViewSelectionDidChange(_ notification: Notification) {
        print("select")
        let obj = notification.object as! NSTableView
        let item = KeyManager.shared.keys[obj.selectedRow]
        detailsView.isHidden = false
        creationDate.stringValue = item.creationDate
        keychainVersion.stringValue = item.keychainVersion
        cipherType.stringValue = item.cipherType
        location.stringValue = item.location
        descriptionKey.stringValue = item.description
        publicKey.stringValue = item.publicKey
        print(obj.selectedRow)
    }
    
}

