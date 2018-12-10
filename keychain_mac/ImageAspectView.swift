//
//  ImageAspectView.swift
//  keychain
//
//  Created by Mikhail Lutskiy on 29/11/2018.
//

import Cocoa

@objc class ImageAspectView: NSImageView {
    
    override var image: NSImage? {
        set {
            self.layer = CALayer()
            self.layer?.contentsGravity = CALayerContentsGravity.resizeAspectFill
            self.layer?.contents = newValue
            self.wantsLayer = true
            
            super.image = newValue
        }
        
        get {
            return super.image
        }
    }
    
}
