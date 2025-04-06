//
//  TexturePage.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


internal let texturePageWidth = 256
internal let texturePageSize = texturePageWidth * texturePageWidth * 3


public struct WKTexturePage: Sendable {
    public static let width = 256
    public static let height = 256
    public static let size = width * height * 3
    
    public private(set) var contents: Data
}


public extension WKTexturePage {
    func generateBGRA8Texture() async -> Data {
        let texturePage = [UInt8](contents)
        var contents = [UInt8](repeating: 255, count: WKTexturePage.width * WKTexturePage.height * 4)
        
        for c in 0 ..< texturePageWidth {
            for r in 0 ..< texturePageWidth {
                let pixelIndex = c * texturePageWidth * 4 + r * 4
                
                let start = c * texturePageWidth * 3 + r * 3
                let r = texturePage[start + 0]
                let g = texturePage[start + 1]
                let b = texturePage[start + 2]
                
                // Check for magenta color
                if r == 255 && g == 0 && b == 255 {
                    contents[pixelIndex + 0] = 0
                    contents[pixelIndex + 1] = 0
                    contents[pixelIndex + 2] = 0
                    contents[pixelIndex + 3] = 0
                }
                else {
                    contents[pixelIndex + 0] = b
                    contents[pixelIndex + 1] = g
                    contents[pixelIndex + 2] = r
                    
                    // 255 is already written
                    //texture.contents[pixelIndex + 3] = 255
                }
            }
        }
        
        return Data(contents)
    }
}
