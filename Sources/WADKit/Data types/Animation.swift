//
//  Animation.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


public struct WKKeyframe: Sendable {
        public var offset: WKVector
    public var rotations: [WKRotation]
}


/// Animation
public struct WKAnimation: Sendable {
    public var frameDuration: Int
    
    public var nextAnimation: Int
    public var nextAnimationFrameIn: Int
    
    internal(set) public var keyframes: [WKKeyframe] = []
}
