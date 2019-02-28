//
//  InfoButton.swift
//  InfoButton
//
//  Created by Kauntey Suryawanshi on 25/06/15.
//  Copyright (c) 2015 Kauntey Suryawanshi. All rights reserved.
//

import Foundation
import Cocoa

//MARK: Extension for making a popover from string
@objc extension NSPopover {

    @objc convenience init(content: String, doesAnimate: Bool) {
        self.init()

        self.behavior = NSPopover.Behavior.transient
        self.animates = doesAnimate
        self.contentViewController = NSViewController()
        self.contentViewController!.view = NSView(frame: NSZeroRect)//remove this ??

        let popoverMargin = CGFloat(20)
        let textField: NSTextField = {
            content in
            let textField = NSTextField(frame: NSZeroRect)

            textField.isEditable = false
            textField.stringValue = content
            textField.isBordered = false
            textField.drawsBackground = false
            textField.sizeToFit()
            textField.setFrameOrigin(NSMakePoint(popoverMargin, popoverMargin))
            return textField
            }(content)

        self.contentViewController!.view.addSubview(textField)
        var viewSize = textField.frame.size;
        viewSize.width += (popoverMargin * 2); viewSize.height += (popoverMargin * 2)
        self.contentSize = viewSize

    }
}
//NSMinXEdge NSMinYEdge NSMaxXEdge NSMaxYEdge
