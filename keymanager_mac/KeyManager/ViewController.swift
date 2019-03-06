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
    @IBOutlet weak var transactionTableView: NSTableView!
    @IBOutlet weak var websocketStatusLabel: NSTextField!
    @IBOutlet weak var creationDate: NSTextFieldCell!
    @IBOutlet weak var keychainVersion: NSTextFieldCell!
    @IBOutlet weak var cipherType: NSTextFieldCell!
//    @IBOutlet weak var location: NSTextFieldCell!
    @IBOutlet weak var descriptionKey: NSTextFieldCell!
    @IBOutlet weak var publicKey: NSTextFieldCell!
    @IBOutlet weak var detailsView: NSView!
    @IBOutlet weak var statusView: NSView!
    @IBOutlet weak var websocketStatusImage: NSImageView!
    @IBOutlet weak var gravatarImage: NSImageView!
    
    var selectedPublicKey = ""
//    @IBOutlet weak var titleView: NSView!
    
    fileprivate enum CellIdentifiers {
        static let LocationCell = "locationId"
        static let key1 = "key1"
        static let dateId = "dateId"
        static let blockchainKey = "blockchainKey"
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.tableView.delegate = self
        self.tableView.dataSource = self
        self.transactionTableView.delegate = self
        self.transactionTableView.dataSource = self
        tableView.reloadData()
        self.view.layer?.backgroundColor = NSColor(red: 242.0/255.0, green: 243.0/255.0, blue: 247.0/255.0, alpha: 1).cgColor
        self.detailsView.wantsLayer = true
        self.detailsView.layer?.backgroundColor = NSColor(red: 248.0/255.0, green: 248.0/255.0, blue: 250.0/255.0, alpha: 1).cgColor
        self.statusView.wantsLayer = true
        self.statusView.layer?.backgroundColor = NSColor.white.cgColor
        
        NotificationCenter.default.addObserver(self, selector: #selector(reloadTableView), name: NSNotification.Name("reloadData"), object: nil)
        
        websocketStatus()
        Timer.scheduledTimer(timeInterval: 5, target: self, selector: #selector(websocketStatus), userInfo: nil, repeats: true)
        // Do any additional setup after loading the view.
    }
    
    @objc func reloadTableView () {
        CPlusPlusBridger().reloadData()
        self.tableView.reloadData()
        print("load")
    }
    
    func updateTableView () {
        
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }
    
    
    func tableView(_ tableView: NSTableView, viewFor tableColumn: NSTableColumn?, row: Int) -> NSView? {
        if tableView == self.tableView {
            var text: String = ""
            var cellIdentifier: String = ""
            
    //        let item = KeyManager.shared.keys[row]
            let key = CPlusPlusBridger().getKeyById(row)
            
            if tableColumn == tableView.tableColumns[0] {
                text = key.name
                cellIdentifier = CellIdentifiers.LocationCell
            }
            
            // 3
            if let cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: nil) as? NSTableCellView {
                cell.textField?.stringValue = text
                return cell
            }
        } else if tableView == self.transactionTableView {
            if selectedPublicKey != "" {
                print("self.transactionTableView")
                let logs = CPlusPlusBridger().getTransactionLog(forPublicKey: selectedPublicKey)
                let log = logs[row] as! Log

                var text: String = ""

                var cellIdentifier: String = ""
                if tableColumn == tableView.tableColumns[0] {
                    switch (log.blockchainType) {
                    case .array:
                        text = "Array"
                    case .unknown:
                        text = "Unknown"
                    case .bitshares:
                        text = "Bitshares"
                    case .ethereum:
                        text = "Ethereum"
                    case .bitcoin:
                        text = "Bitcoin"
                    case .rawhash:
                        text = "Rawhash"
                    }
                    cellIdentifier = CellIdentifiers.blockchainKey
                }
                if tableColumn == tableView.tableColumns[1] {
                    text = getSubstrStr(str: log.transaction)
                    cellIdentifier = CellIdentifiers.key1
                }
                if tableColumn == tableView.tableColumns[2] {
                    text = "\(log.signTime)"
                    cellIdentifier = CellIdentifiers.dateId
                }
                
                // 3
                if let cell = tableView.makeView(withIdentifier: NSUserInterfaceItemIdentifier(rawValue: cellIdentifier), owner: nil) as? NSTableCellView {
                    cell.textField?.stringValue = text
                    return cell
                }
            }
        }
        return nil
        
    }
        
    func numberOfRows(in tableView: NSTableView) -> Int {
        if tableView == self.tableView {
            return CPlusPlusBridger().getKeyfilesCount()
        } else {
            if selectedPublicKey != "" {
                print("numberOfRows")
                return CPlusPlusBridger().getTransactionLog(forPublicKey: selectedPublicKey).count
            } else {
                return 0
            }
        }
    }
    
    @objc func websocketStatus() {
        WebsocketChecker.checkForRunningWebsocketd { (isRunning) in
            self.websocketStatusLabel.stringValue = isRunning ? "Running" : "Not started"
            self.websocketStatusImage.image = isRunning ? #imageLiteral(resourceName: "working") : #imageLiteral(resourceName: "stoped")
        }
    }
    
    func tableView(_ tableView: NSTableView, didClick tableColumn: NSTableColumn) {
        print("click")
    }
    
    func tableView(_ tableView: NSTableView, rowViewForRow row: Int) -> NSTableRowView? {
        return TableRowView()
    }

    func tableViewSelectionDidChange(_ notification: Notification) {
        let obj = notification.object as! NSTableView
            if obj == self.tableView {
            let item = CPlusPlusBridger().getKeyById(obj.selectedRow)
            detailsView.isHidden = false
            let formatter = DateFormatter()
            formatter.dateFormat = "MMM dd, YYYY, HH:mm:ss"
            creationDate.stringValue = formatter.string(from: item.createTime)
            keychainVersion.stringValue = item.keychainVersion
            cipherType.stringValue = item.cipherType
//            location.stringValue = ""
            descriptionKey.stringValue = item.descriptionKey
            publicKey.stringValue = getSubstrStr(str: item.publicKey)
            selectedPublicKey = item.publicKey
            gravatarImage.image = Identicon().icon(from: item.publicKey, size: CGSize(width: 122, height: 122), scale: 2.0)
            transactionTableView.reloadData()
            print(obj.selectedRow)
        }
    }
    
    func getSubstrStr(str: String) -> String{
        let index = str.index(str.startIndex, offsetBy: 32)
        let substring = str[..<index] // Hello
        
        let indexLast = str.index(str.endIndex, offsetBy: -8)
        let substringLast = str[indexLast...] // playground
        return substring + "<...>" + substringLast
    }
    
}

