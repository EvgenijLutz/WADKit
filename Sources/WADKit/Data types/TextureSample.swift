//
//  TextureSample.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation


// half pixel offset
fileprivate let tinyOffset: Float = 1 / 256 / 2


struct RawTextureSample {
    var rawX: UInt8
    var rawY: UInt8
    var page: UInt16
    var flipX: Int8
    var addW: UInt8
    var flipY: Int8
    var addH: UInt8
    
    
    var left: Float {
        if flipX == 0 {
            return Float(Int(rawX) + Int(addW) + 1) / 256 - tinyOffset
        }
        // Prevent arithmetic overflow
        return Float(rawX) / 256 + tinyOffset
        
    }
    
    var right: Float {
        if flipX == 0 {
            return Float(rawX) / 256 + tinyOffset
        }
        return Float(Int(rawX) + Int(addW) + 1) / 256 - tinyOffset
        
    }
    
    
    var top: Float {
        if flipY == 0 {
            return Float(Int(rawY) + Int(addH) + 1) / 256 - tinyOffset
        }
        return Float(rawY) / 256 + tinyOffset
        
    }
    
    var bottom: Float {
        if flipY == 0 {
            return Float(rawY) / 256 + tinyOffset
        }
        return Float(Int(rawY) + Int(addH) + 1) / 256 - tinyOffset
        
    }
}

public class TextureSample {
    public private(set) weak var owner: WAD?
    internal var raw: RawTextureSample
    
    public var page: TexturePage? {
        guard let owner else { return nil }
        guard raw.page < owner.texturePages.count else { return nil }
        return owner.texturePages[Int(raw.page)]
    }
    
    
    internal init(owner: WAD, raw: RawTextureSample) {
        self.owner = owner
        self.raw = raw
    }
}
