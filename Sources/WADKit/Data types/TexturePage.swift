//
//  TexturePage.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation


public class TexturePage {
    public private(set) weak var owner: WAD?
    public private(set) var contents: Data
    
    var samples: [TextureSample] {
        guard let owner else { return [] }
        guard let page = owner.texturePages.firstIndex(where: { $0 === self }) else { return [] }
        return owner.textureSamples.filter { $0.raw.page == page }
    }
    
    
    init(owner: WAD, contents: Data) {
        self.owner = owner
        self.contents = contents
    }
}
