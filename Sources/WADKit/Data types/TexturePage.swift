//
//  TexturePage.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


public class WKTexturePage {
    public private(set) weak var wad: WAD?
    public private(set) var contents: Data
    
    public var samples: [WKTextureSample] {
        guard let wad else { return [] }
        guard let page = wad.texturePages.firstIndex(where: { $0 === self }) else { return [] }
        return wad.textureSamples.filter { $0.raw.page == page }
    }
    
    
    init(wad: WAD, contents: Data) {
        self.wad = wad
        self.contents = contents
    }
}
