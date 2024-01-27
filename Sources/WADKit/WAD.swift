//
//  WAD.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation


public class WAD {
    public enum LoadError: Error {
        case custom(_ message: String)
    }
    
    private(set) var version: Version = .TombRaiderTheLastRevelation
    private(set) var texturePages: [TexturePage] = []
    private(set) var textureSamples: [TextureSample] = []
    private(set) var meshes: [Mesh] = []
    private(set) var animations: [Animation] = []
    
    public init() {
        //version = .TombRaiderTheLastRevelation
    }
    
    public func createTexturePage(withContentsOf data: Data) -> TexturePage {
        let texturePage = TexturePage(owner: self, contents: data)
        texturePages.append(texturePage)
        return texturePage
    }
    
    internal func createTextureSample(withRawTextureSample rawSample: RawTextureSample) -> TextureSample {
        let textureSample = TextureSample(owner: self, raw: rawSample)
        textureSamples.append(textureSample)
        return textureSample
    }
    
    public func createAnimation() -> Animation {
        let animation = Animation(owner: self)
        animations.append(animation)
        return animation
    }
    
    public static func fromFileURL(url: URL) throws -> WAD {
        let data = try Data(contentsOf: url)
        let reader = DataReader(with: data)
        
        let wad = WAD()
        
        // MARK: - Section 1 - Version
        wad.version = try reader.read()
        
        // MARK: - Section 2 - Textures
        let numTextureSamples: UInt32 = try reader.read()
        wadLog("Number of texture samples: \(numTextureSamples)")
        
        var rawTextureSamples: [RawTextureSample] = []
        for _ in 0 ..< numTextureSamples {
            rawTextureSamples.append(
                RawTextureSample(
                    x: try reader.read(),
                    y: try reader.read(),
                    page: try reader.read(),
                    flipX: try reader.read(),
                    addW: try reader.read(),
                    flipY: try reader.read(),
                    addH: try reader.read()
                )
            )
        }
        
        let texturePageSize: UInt32 = 256 * 256 * 3
        let textureMapsBytes: UInt32 = try reader.read()
        wadLog("Total size of texture maps: \(textureMapsBytes)")
        guard textureMapsBytes % texturePageSize == 0 else {
            throw LoadError.custom("Wrong total size of texture maps")
        }
        let numTexturePages = textureMapsBytes / texturePageSize
        wadLog("Number of texture pages: \(numTexturePages)")
        var texturePages: [Data] = []
        for _ in 0..<numTexturePages {
            texturePages.append(try reader.readData(ofLength: texturePageSize))
        }
        
        // MARK: - Section 3 - Meshes
        let numMeshPointers: UInt32 = try reader.read()
        wadLog("Number of mesh poitners: \(numMeshPointers)")
        var meshPointers: [UInt32] = []
        for _ in 0..<numMeshPointers {
            meshPointers.append(try reader.read())
        }
        
        let numMeshesWords: UInt32 = try reader.read()
        let animationsOffset = reader.offset + Int(numMeshesWords) * 2
        wadLog("Number of mesh buffer words: \(numMeshesWords) (\(numMeshesWords * 2) bytes)")
        
        repeat {
            // Bounding sphere
            let boundingSphere = BoundingSphere(
                rawCenterX: try reader.read(),
                rawCenterY: try reader.read(),
                rawCenterZ: try reader.read(),
                rawRadius: try reader.read(),
                unknown: try reader.read()
            )
            //wadLog("\(boundingSphere)")
            
            // Vertices
            let numVertives: UInt16 = try reader.read()
            //wadLog("Number of vertices: \(numVertives)")
            
            var vertices: [Vertex] = []
            for _ in 0 ..< numVertives {
                vertices.append(
                    Vertex(
                        rawX: try reader.read(),
                        rawY: try reader.read(),
                        rawZ: try reader.read()
                    )
                )
            }
            
            // Normals/Shades
            let numNormalsOrShades: Int16 = try reader.read()
            //wadLog("Number of normals or shades: \(numNormalsOrShades)")
            
            var normals: [Normal] = []
            var shades: [Shade] = []
            
            if numNormalsOrShades > 0 {
                for _ in 0 ..< numNormalsOrShades {
                    normals.append(
                        Normal(
                            rawX: try reader.read(),
                            rawY: try reader.read(),
                            rawZ: try reader.read()
                        )
                    )
                }
            }
            else {
                for _ in 0 ..< -numNormalsOrShades {
                    shades.append(Shade(rawValue: try reader.read()))
                }
            }
            
            // Polygons
            let numPolygons: UInt16 = try reader.read()
            //wadLog("Number of polygons: \(numPolygons)")
            var polygons: [Polygon] = []
            var numQuads = 0
            for _ in 0 ..< numPolygons {
                let polygon: Polygon = try reader.read()
                if case .quad(_) = polygon.shape {
                    numQuads += 1
                }
                polygons.append(polygon)
            }
            
            // Add mesh
            wad.meshes.append(
                Mesh(
                    owner: wad,
                    boundingSphere: boundingSphere,
                    vertices: vertices,
                    normals: normals,
                    shades: shades,
                    polygons: polygons
                )
            )
            
            // Skip excess bytes
            //wadLog("Total number of quads: \(numQuads)")
            if numQuads % 2 == 1 {
                //wadLog("Skip 2 bytes because number of quads is odd...")
                reader.skip(2)
            }
        } while reader.offset < animationsOffset
        wadLog("Meshes read: \(wad.meshes.count)")
        
        
        // MARK: - Section 4 - Animations
        wadLog("Jumping into the animation block from \(reader.offset) to \(animationsOffset)")
        reader.set(animationsOffset)
        let numAnimations: UInt32 = try reader.read()
        wadLog("Number of animations: \(numAnimations)")
        
        struct RawAnimation {
            var keyframeOffset: UInt32
            var frameDuration: UInt8
            var keyframeSize: UInt8
            var stateId: UInt16
            var unknown1: UInt16
            
            var speed: Int16
            var acceleration: Int32
            var unknown2: UInt64
            
            var frameStart: UInt16
            var frameEnd: UInt16
            
            var nextAnimation: UInt16
            var frameIn: UInt16
            
            var numStateChanges: UInt16
            var changesIndex: UInt16
            
            var numCommands: UInt16
            var commandOffset: UInt16
            
            
            var numberOfFrames: Int {
                return Int(frameEnd) - Int(frameStart) + 1
            }
            
            //var numKeyframes: Int {
            //    return
            //}
        }
        
        var rawAnimations: [RawAnimation] = []
        for _ in 0 ..< numAnimations {
            let animation: RawAnimation = RawAnimation(
                keyframeOffset: try reader.read(),
                frameDuration: try reader.read(),
                keyframeSize: try reader.read(),
                stateId: try reader.read(),
                unknown1: try reader.read(),
                speed: try reader.read(),
                acceleration: try reader.read(),
                unknown2: try reader.read(),
                frameStart: try reader.read(),
                frameEnd: try reader.read(),
                nextAnimation: try reader.read(),
                frameIn: try reader.read(),
                numStateChanges: try reader.read(),
                changesIndex: try reader.read(),
                numCommands: try reader.read(),
                commandOffset: try reader.read()
            )
            rawAnimations.append(animation)
        }
        wadLog("Animations read: \(rawAnimations.count)")
        
        // State changes
        let numStateChanges: UInt32 = try reader.read()
        wadLog("Number of state changes: \(numStateChanges)")
        
        struct StateChange {
            var stateId: UInt16
            var numDispatches: UInt16
            var dispatchesIndex: UInt16
        }
        var stateChanges: [StateChange] = []
        for _ in 0 ..< numStateChanges {
            stateChanges.append(
                StateChange(
                    stateId: try reader.read(),
                    numDispatches: try reader.read(),
                    dispatchesIndex: try reader.read()
                )
            )
        }
        
        // Dispatches
        let numDispatches: UInt32 = try reader.read()
        wadLog("Number of dispatches: \(numDispatches)")
        
        struct RawDispatch {
            var inRange: UInt16
            var outRange: UInt16
            var nextAnim: UInt16
            var frameIn: UInt16
        }
        var rawDispatches: [RawDispatch] = []
        for _ in 0 ..< numDispatches {
            rawDispatches.append(
                RawDispatch(
                    inRange: try reader.read(),
                    outRange: try reader.read(),
                    nextAnim: try reader.read(),
                    frameIn: try reader.read()
                )
            )
        }
        
        // Commands
        let numCommandsWords: UInt32 = try reader.read()
        wadLog("Number of commands words: \(numCommandsWords) (\(numCommandsWords * 2) bytes)")
        
        struct RawCommand {
            var command: UInt16
            var operator1: UInt16
            var operator2: UInt16
            var operator3: UInt16
        }
        let commandsBuffer = try reader.readData(ofLength: numCommandsWords * 2)
        //let commandsReader = DataReader(with: commandsBuffer)
        wadLog("Read a commands buffer of \(commandsBuffer.count) bytes")
        
        // Links
        let numLinksDWords: UInt32 = try reader.read()
        let linksBufferSize = Int(numLinksDWords * 4)
        let keyframesPackageAddress = reader.offset + linksBufferSize
        wadLog("Number of links dwords: \(numLinksDWords) (\(linksBufferSize) bytes)")
        
        struct RawLink {
            var operationCode: Int32
            var dx: Int32
            var dy: Int32
            var dz: Int32
        }
        let linksBuffer: Data = try reader.readData(ofLength: linksBufferSize)
        wadLog("Read a links buffer of \(linksBuffer.count) bytes")
        /*var rawLinks: [RawLink] = []
         repeat {
         let operationCode: Int32 = try reader.read()
         if operationCode < 0 || operationCode > 3 {
         wadLog("links read: \(rawLinks.count), last link: \(rawLinks.last)")
         throw LoadError.custom("Wrong operation code: \(operationCode)")
         }
         rawLinks.append(
         RawLink(
         operationCode: operationCode,
         dx: try reader.read(),
         dy: try reader.read(),
         dz: try reader.read()
         )
         )
         } while reader.offset + 16 <= keyframesPackageAddress
         wadLog(rawLinks.last)*/
        
        
        // Keyframes
        wadLog("Jumping into the keyframes block from \(reader.offset) to \(keyframesPackageAddress)")
        reader.set(keyframesPackageAddress)
        let numKeyframesWords: UInt32 = try reader.read()
        let modelsSectionAddress = reader.offset + Int(numKeyframesWords * 2)
        wadLog("Number of keyframes words: \(numKeyframesWords) (\(numKeyframesWords * 2) bytes)")
        
        let keyframesBuffer = try reader.readData(ofLength: numKeyframesWords * 2)
        //let keyframesReader = DataReader(with: keyframesBuffer)
        wadLog("Read a keyframes buffer of \(keyframesBuffer.count) bytes")
        
        
        // MARK: - Section 5 - Models
        wadLog("Jumping into the models block from \(reader.offset) to \(modelsSectionAddress)")
        reader.set(modelsSectionAddress)
        
        // Movables
        let numMovables: UInt32 = try reader.read()
        wadLog("Number of movables: \(numMovables)")
        
        struct RawMovable {
            var objectId: MovableIdentifier
            var numPointers: UInt16
            var pointersIndex: UInt16
            var linksIndex: UInt32
            var keyframesOffset: UInt32
            var animsIndex: Int16
        }
        var rawMovables: [RawMovable] = []
        for _ in 0 ..< numMovables {
            rawMovables.append(
                RawMovable(
                    objectId: try reader.read(),
                    numPointers: try reader.read(),
                    pointersIndex: try reader.read(),
                    linksIndex: try reader.read(),
                    keyframesOffset: try reader.read(),
                    animsIndex: try reader.read()
                )
            )
        }
        
        
        // Static objects
        let numStaticObjects: UInt32 = try reader.read()
        wadLog("Number of static objects: \(numStaticObjects)")
        
        struct RawStaticObject {
            var objectId: UInt32
            var pointersIndex: UInt16
            
            var vx1: UInt16
            var vx2: UInt16
            var vy1: UInt16
            var vy2: UInt16
            var vz1: UInt16
            var vz2: UInt16
            
            var cx1: UInt16
            var cx2: UInt16
            var cy1: UInt16
            var cy2: UInt16
            var cz1: UInt16
            var cz2: UInt16
            
            var flags: UInt16
        }
        var rawStaticObjects: [RawStaticObject] = []
        for _ in 0 ..< numStaticObjects {
            rawStaticObjects.append(
                RawStaticObject(
                    objectId: try reader.read(),
                    pointersIndex: try reader.read(),
                    
                    vx1: try reader.read(),
                    vx2: try reader.read(),
                    vy1: try reader.read(),
                    vy2: try reader.read(),
                    vz1: try reader.read(),
                    vz2: try reader.read(),
                    
                    cx1: try reader.read(),
                    cx2: try reader.read(),
                    cy1: try reader.read(),
                    cy2: try reader.read(),
                    cz1: try reader.read(),
                    cz2: try reader.read(),
                    
                    flags: try reader.read()
                )
            )
        }
        
        // Happy end
        if reader.offset == reader.data.count {
            wadLog("Successfully reached the end of file 😌")
        }
        
        
        // MARK: - Fill wad contents
        
        // Texture pages
        for page in texturePages {
            _ = wad.createTexturePage(withContentsOf: page)
        }
        
        
        // Texture samples
        for sample in rawTextureSamples {
            _ = wad.createTextureSample(withRawTextureSample: sample)
        }
        
        
        // Meshes
        // already done
        
        
        // Animations
        for rawAnimation in rawAnimations {
            let animation = wad.createAnimation()
            
            
            if rawAnimation.acceleration > 1 {
                animation.lala()
            }
        }
        
        
        return wad
    }
    
    //func serialize() -> Data {
    //    //
    //}
}

extension WAD: CustomStringConvertible {
    public var description: String {
        "WAD v\(version). Textures: \(texturePages.count), samples: \(textureSamples.count), meshes: \(meshes.count), animations: \(animations.count)"
    }
}

