//
//  TableRowView.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 23/01/2019.
//  Copyright © 2019 Mikhail Lutskii. All rights reserved.
//

import Cocoa

class TableRowView: NSTableRowView {

    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
    }
    
    override func drawSelection(in dirtyRect: NSRect) {
        if self.selectionHighlightStyle != .none {
            let selectionRect = NSInsetRect(self.bounds, 0, 0)
            NSColor(red: 166.0/255.0, green: 178.0/255.0, blue: 200.0/255.0, alpha: 1).setFill()
            let selectionPath = NSBezierPath.init(roundedRect: selectionRect, xRadius: 0, yRadius: 0)
            selectionPath.fill()
//            selectionPath.stroke()
        }
    }
    
}
