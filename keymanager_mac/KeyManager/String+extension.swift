//
//  String+extension.swift
//  KeyManager
//
//  Created by Mikhail Lutskiy on 26/12/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Foundation

extension String {
    func fileName() -> String {
        return NSURL(fileURLWithPath: self).deletingPathExtension?.lastPathComponent ?? ""
    }
    
    func fileExtension() -> String {
        return NSURL(fileURLWithPath: self).pathExtension ?? ""
    }
}
