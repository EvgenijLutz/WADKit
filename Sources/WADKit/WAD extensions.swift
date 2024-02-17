//
//  WAD extensions.swift
//  WADKit
//
//  Created by Eugen Lutz on 31.01.24.
//

import Foundation


internal let texturePageWidth = 256
internal let texturePageSize = texturePageWidth * texturePageWidth * 3


public class FuckingTextureHolder {
    public var contents: [UInt8]
    
    init(textureWidth: Int) {
        contents = .init(repeating: 255, count: textureWidth * textureWidth * 4)
    }
}


public struct TexturePageRemapInfo {
    public let pageIndex: Int
    public let textureIndex: Int
    public let uvMagnifier: Float
    public let offsetU: Float
    public let offsetV: Float
    
    
    public init(pageIndex: Int, textureIndex: Int, uvMagnifier: Float, offsetU: Float, offsetV: Float) {
        self.pageIndex = pageIndex
        self.textureIndex = textureIndex
        self.uvMagnifier = uvMagnifier
        self.offsetU = offsetU
        self.offsetV = offsetV
    }
}


public struct CombinedTexturePages {
    public let textures: [FuckingTextureHolder]
    public let textureWidth: Int
    public let remapInfo: [TexturePageRemapInfo]
}


public extension WAD {
    func generateTextureRemapInfo(pagesPerRow: Int) -> [TexturePageRemapInfo] {
        let numPagesInTexture = pagesPerRow * pagesPerRow
        
        var remapInfo: [TexturePageRemapInfo] = []
        for index in texturePages.indices {
            let indexInTexture = index % numPagesInTexture
            let row = indexInTexture % pagesPerRow
            let column = indexInTexture / pagesPerRow
            //wadImportLog("\(indexInTexture) -> \(row) : \(column)")
            
            remapInfo.append(
                TexturePageRemapInfo(
                    pageIndex: index,
                    textureIndex: index / numPagesInTexture,
                    uvMagnifier: 1 / Float(pagesPerRow),
                    offsetU: 1 / Float(pagesPerRow) * Float(row),
                    offsetV: 1 / Float(pagesPerRow) * Float(column)
                )
            )
        }
        
        return remapInfo
    }
    
    /// Generates a bundle of textures with BGRA8 pixel format and (texture page index) -> (texture index) remap info.
    func generateCombinedTexturePages(pagesPerRow: Int) -> CombinedTexturePages {
        let remapInfo = generateTextureRemapInfo(pagesPerRow: pagesPerRow)
        
        let pagesPerRow = 8
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
            textureWidth: textureWidth,
            remapInfo: remapInfo
        )
    }
}
