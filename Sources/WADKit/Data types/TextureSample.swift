//
//  TextureSample.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


// half pixel offset
fileprivate let tinyOffset: Float = 1 / 256 / 2
//fileprivate let tinyOffset: Float = 1 / 256 / 8


struct RawTextureSample: Sendable {
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

extension RawTextureSample: Equatable {
    //
}


public struct WKTextureSample: Sendable {
    internal var raw: RawTextureSample
    
    public var page: Int {
        return Int(raw.page)
    }
}

extension WKTextureSample: Equatable {
    //
}
