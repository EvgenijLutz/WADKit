//
//  WAD extensions.swift
//  WADKit
//
//  Created by Evgenij Lutz on 31.01.24.
//

import Foundation


public class FuckingTextureHolder: @unchecked Sendable {
    fileprivate(set) public var contents: [UInt8]
    
    init(textureWidth: Int) {
        contents = .init(repeating: 255, count: textureWidth * textureWidth * 4)
    }
}


public struct TexturePageRemapInfo: Sendable {
    public let pageIndex: Int
    public let textureIndex: Int
    public let uvMagnifier: Float
    public let offsetU: Float
    public let offsetV: Float
}


public struct CombinedTexturePages: Sendable {
    public let textures: [FuckingTextureHolder]
    public let width: Int
    
    /// Always equals to `width`.
    public var height: Int {
        width
    }
    
    public let remapInfo: [TexturePageRemapInfo]
}


public extension WAD {
    func generateTextureRemapInfo(pagesPerRow: Int) -> [TexturePageRemapInfo] {
        let numPagesInTexture = pagesPerRow * pagesPerRow
        return texturePages.indices.map { index in
            let indexInTexture = index % numPagesInTexture
            let row = indexInTexture % pagesPerRow
            let column = indexInTexture / pagesPerRow
            //wadImportLog("\(indexInTexture) -> \(row) : \(column)")
            
            return TexturePageRemapInfo(
                pageIndex: index,
                textureIndex: index / numPagesInTexture,
                uvMagnifier: 1 / Float(pagesPerRow),
                offsetU: 1 / Float(pagesPerRow) * Float(row),
                offsetV: 1 / Float(pagesPerRow) * Float(column)
            )
        }
    }
    
    /// Generates a bundle of textures with BGRA8 pixel format and (texture page index) -> (texture index) remap info.
    func generateCombinedTexturePages(pagesPerRow: Int) async -> CombinedTexturePages {
        let remapInfo = generateTextureRemapInfo(pagesPerRow: pagesPerRow)
        
        let pagesPerTexture = pagesPerRow * pagesPerRow
        let textureWidth = pagesPerRow * texturePageWidth
        var textures: [FuckingTextureHolder] = []
        
        
        let numTextures = (remapInfo.last?.textureIndex ?? 0) + 1
        for _ in 0 ..< numTextures {
            textures.append(FuckingTextureHolder(textureWidth: textureWidth))
        }
        
        
        for (infoIndex, info) in remapInfo.enumerated() {
            let texture = textures[info.textureIndex]
            
            let pageInTextureIndex = infoIndex % pagesPerTexture
            let columnPage = pageInTextureIndex / pagesPerRow
            let rowPage = pageInTextureIndex % pagesPerRow
            // Increase performance by copying byte by byte instead of using Data. Using array reduced a test WAD file loading time from 1.4 seconds to 0.48
            let texturePage = [UInt8](texturePages[infoIndex].contents)
            
            for c in 0 ..< texturePageWidth {
                for r in 0 ..< texturePageWidth {
                    let pixelIndex =
                    (columnPage * texturePageWidth + c) * textureWidth * 4 +
                    (rowPage * texturePageWidth + r) * 4
                    
                    let start = c * texturePageWidth * 3 + r * 3
                    let r = texturePage[start + 0]
                    let g = texturePage[start + 1]
                    let b = texturePage[start + 2]
                    
                    // Check for magenta color
                    if r == 255 && g == 0 && b == 255 {
                        texture.contents[pixelIndex + 0] = 0
                        texture.contents[pixelIndex + 1] = 0
                        texture.contents[pixelIndex + 2] = 0
                        texture.contents[pixelIndex + 3] = 0
                    }
                    else {
                        texture.contents[pixelIndex + 0] = b
                        texture.contents[pixelIndex + 1] = g
                        texture.contents[pixelIndex + 2] = r
                        
                        // 255 is already written
                        //texture.contents[pixelIndex + 3] = 255
                    }
                }
            }
        }
        
        return .init(
            textures: textures,
            width: textureWidth,
            remapInfo: remapInfo
        )
    }
}
