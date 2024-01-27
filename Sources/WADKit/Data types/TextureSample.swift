//
//  TextureSample.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation


struct RawTextureSample {
    var x: UInt8
    var y: UInt8
    var page: UInt16
    var flipX: Int8
    var addW: UInt8
    var flipY: Int8
    var addH: UInt8
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
