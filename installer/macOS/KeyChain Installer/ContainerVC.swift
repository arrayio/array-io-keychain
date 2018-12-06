//
//  ContainerVC.swift
//  Keychain Installer
//
//  Created by Mikhail Lutskiy on 22/11/2018.
//  Copyright © 2018 Mikhail Lutskii. All rights reserved.
//

import Cocoa

class ContainerVC: NSPageController, NSPageControllerDelegate {
    
    var viewArray = ["one", "two", "three"]
    @IBOutlet weak var nextButton: NSButton!
    @IBOutlet weak var backButton: NSButton!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        delegate = self
        self.arrangedObjects = viewArray
        self.transitionStyle = .horizontalStrip
        self.backButton.isEnabled = false
        
        NotificationCenter.default.addObserver(self, selector: #selector(navigateForward(_:)), name: .navigationForward, object: nil)
        print("Start")
    }
    
    /// Function overrided for disable trackpad gestures for navigationForward/backward
    ///
    /// - Parameter event: event
    override func scrollWheel(with event: NSEvent) {
        
    }
    
    func pageController(_ pageController: NSPageController, viewControllerForIdentifier identifier: NSPageController.ObjectIdentifier) -> NSViewController {
//        print(identifier.rawValue)
        switch identifier {
        case "one":
            return self.storyboard?.instantiateController(withIdentifier: "WelcomeVC") as! WelcomeVC
        case "two":
            return self.storyboard?.instantiateController(withIdentifier: "SelectPathVC") as! SelectPathVC
        case "three":
            return self.storyboard?.instantiateController(withIdentifier: "ProgressVC") as! ProgressVC
        default:
            return self.storyboard?.instantiateController(withIdentifier: identifier) as! NSViewController
        }
    }
    
    func pageController(_ pageController: NSPageController, identifierFor object: Any) -> NSPageController.ObjectIdentifier {
        return object as! NSPageController.ObjectIdentifier
    }
    
    func pageControllerDidEndLiveTransition(_ pageController: NSPageController) {
        self.completeTransition()
    }
    
    override func navigateForward(_ sender: Any?) {
        super.navigateForward(sender)
        buttonHiddenRules()
    }
    
    fileprivate func buttonHiddenRules() {
        switch self.selectedIndex {
        case 0:
            self.backButton.isEnabled = false
            self.nextButton.isHidden = false
            break
        case 1:
            self.backButton.isEnabled = true
            self.nextButton.isHidden = false
            break
        case 2:
            self.backButton.isEnabled = true
            self.nextButton.isHidden = true
        default:
            break
        }
    }
    
    override func navigateBack(_ sender: Any?) {
        super.navigateBack(sender)
        buttonHiddenRules()
    }
    
}
