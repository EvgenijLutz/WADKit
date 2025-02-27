//
//  WAD.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation
import os


public enum WADError: Error, Sendable {
    case ownerNotFound
    
    case wrongMeshPointerIndex
    case wrongMeshOffset
    
    case unknownJointLinkOperationCode(_ code: Int32)
    
    case other(_ message: String)
}


private let wadLock = OSAllocatedUnfairLock()
internal func withWadLock<T: Sendable>(_ block: @Sendable () throws -> T) rethrows -> T {
    return try wadLock.withLock {
        try block()
    }
}


public class WAD: @unchecked Sendable {
    
    public private(set) var version: WADVersion = .TombRaiderTheLastRevelation
    public private(set) var texturePages: [WKTexturePage] = []
    public private(set) var textureSamples: [TextureSample] = []
    public private(set) var meshes: [WKMesh] = []
    public private(set) var animations: [WKAnimation] = []
    public private(set) var movables: [WKMovable] = []
    public private(set) var staticObjects: [WKStaticObject] = []
    
    public init() {
        //version = .TombRaiderTheLastRevelation
    }
    
    public func createTexturePage(withContentsOf data: Data) -> WKTexturePage {
        let texturePage = WKTexturePage(wad: self, contents: data)
        texturePages.append(texturePage)
        return texturePage
    }
    
    internal func createTextureSample(withRawTextureSample rawSample: RawTextureSample) -> TextureSample {
        let textureSample = TextureSample(owner: self, raw: rawSample)
        textureSamples.append(textureSample)
        return textureSample
    }
    
    public func createAnimation() -> WKAnimation {
        let animation = WKAnimation(wad: self)
        animations.append(animation)
        return animation
    }
    
    
//    public func replaceContents() async throws {
//        
//    }
    
    
    public static func fromFileURL(url: URL) async throws -> WAD {
        let data = try Data(contentsOf: url)
        let reader = DataReader(with: data)
        
        let wad = WAD()
        
        // MARK: - Section 1 - Version
        wad.version = try reader.read()
        
        // MARK: - Section 2 - Textures
        let numTextureSamples: UInt32 = try reader.read()
        wadImportLog("Number of texture samples: \(numTextureSamples)")
        
        var rawTextureSamples: [RawTextureSample] = []
        for _ in 0 ..< numTextureSamples {
            rawTextureSamples.append(
                RawTextureSample(
                    rawX: try reader.read(),
                    rawY: try reader.read(),
                    page: try reader.read(),
                    flipX: try reader.read(),
                    addW: try reader.read(),
                    flipY: try reader.read(),
                    addH: try reader.read()
                )
            )
        }
        
        let textureMapsBytes: UInt32 = try reader.read()
        wadImportLog("Total size of texture maps: \(textureMapsBytes)")
        guard textureMapsBytes % UInt32(texturePageSize) == 0 else {
            throw WADError.other("Wrong total size of texture maps")
        }
        let numTexturePages = textureMapsBytes / UInt32(texturePageSize)
        wadImportLog("Number of texture pages: \(numTexturePages)")
        var texturePages: [Data] = []
        for _ in 0 ..< numTexturePages {
            texturePages.append(try reader.readData(ofLength: texturePageSize))
        }
        
        // MARK: - Section 3 - Meshes
        let numMeshPointers: UInt32 = try reader.read()
        wadImportLog("Number of mesh poitners: \(numMeshPointers)")
        var meshPointers: [UInt32] = []
        for _ in 0 ..< numMeshPointers {
            meshPointers.append(try reader.read())
        }
        
        let numMeshesWords: UInt32 = try reader.read()
        let animationsOffset = reader.offset + Int(numMeshesWords) * 2
        wadImportLog("Number of mesh buffer words: \(numMeshesWords) (\(numMeshesWords * 2) bytes)")
        
        let meshStartOffset = reader.offset
        var meshOffsets: [Int] = []
        repeat {
            meshOffsets.append(reader.offset - meshStartOffset)
            
            // Bounding sphere
            let boundingSphere = BoundingSphere(
                rawCenterX: try reader.read(),
                rawCenterY: try reader.read(),
                rawCenterZ: try reader.read(),
                rawRadius: try reader.read(),
                unknown: try reader.read()
            )
            //wadImportLog("\(boundingSphere)")
            
            // Vertices
            let numVertives: UInt16 = try reader.read()
            //wadImportLog("Number of vertices: \(numVertives)")
            
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
            //wadImportLog("Number of normals or shades: \(numNormalsOrShades)")
            
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
            //wadImportLog("Number of polygons: \(numPolygons)")
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
                .init(
                    wad: wad,
                    boundingSphere: boundingSphere,
                    vertices: vertices,
                    normals: normals,
                    shades: shades,
                    polygons: polygons
                )
            )
            
            // Skip excess bytes
            //wadImportLog("Total number of quads: \(numQuads)")
            if numQuads % 2 == 1 {
                //wadImportLog("Skip 2 bytes because number of quads is odd...")
                reader.skip(2)
            }
        } while reader.offset < animationsOffset
        wadImportLog("Meshes read: \(wad.meshes.count)")
        
        
        // MARK: - Section 4 - Animations
        wadImportLog("Jumping into the animation block from \(reader.offset) to \(animationsOffset)")
        reader.set(animationsOffset)
        let numAnimations: UInt32 = try reader.read()
        wadImportLog("Number of animations: \(numAnimations)")
        
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
        wadImportLog("Animations read: \(rawAnimations.count)")
        
        // State changes
        let numStateChanges: UInt32 = try reader.read()
        wadImportLog("Number of state changes: \(numStateChanges)")
        
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
        wadImportLog("Number of dispatches: \(numDispatches)")
        
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
        wadImportLog("Number of commands words: \(numCommandsWords) (\(numCommandsWords * 2) bytes)")
        
        struct RawCommand {
            var command: UInt16
            var operator1: UInt16
            var operator2: UInt16
            var operator3: UInt16
        }
        let commandsBuffer = try reader.readData(ofLength: numCommandsWords * 2)
        //let commandsReader = DataReader(with: commandsBuffer)
        wadImportLog("Read a commands buffer of \(commandsBuffer.count) bytes")
        
        // Links
        let numLinksDWords: UInt32 = try reader.read()
        let linksBufferSize = Int(numLinksDWords * 4)
        let keyframesPackageAddress = reader.offset + linksBufferSize
        wadImportLog("Number of links dwords: \(numLinksDWords) (\(linksBufferSize) bytes)")
        
        struct RawLink {
            var operationCode: Int32
            var dx: Int32
            var dy: Int32
            var dz: Int32
            
            var position: Vector3f {
                return .init(
                    x: -1 / 1024 * Float(dx),
                    y: -1 / 1024 * Float(dy),
                    z: -1 / 1024 * Float(dz)
                )
            }
        }
        let linksBuffer: Data = try reader.readData(ofLength: linksBufferSize)
        // You cannot read raw links here, because the link buffer is broken in some wads, you need to directly read it from offsets
        wadImportLog("Read a links buffer of \(linksBufferSize) bytes")
        
        let linksReader = DataReader(with: linksBuffer)
        func readLink(at offset: UInt32) throws -> RawLink {
            linksReader.set(offset)
            
            let operationCode: Int32 = try linksReader.read()
            if operationCode < 0 || operationCode > 3 {
                throw WADError.other("Wrong operation code: \(operationCode)")
            }
            
            return RawLink(
                operationCode: operationCode,
                dx: try linksReader.read(),
                dy: try linksReader.read(),
                dz: try linksReader.read()
            )
        }
        
        //var rawLinks: [RawLink] = []
        //var linksRead = 0
        //repeat {
        //    linksRead += 1
        //    let operationCode: Int32 = try reader.read()
        //    if operationCode < 0 || operationCode > 3 {
        //        if let last = rawLinks.last {
        //            wadImportLog("links read: \(rawLinks.count), last link: \(last)")
        //        }
        //        continue
        //        //throw WADError.other("Wrong operation code: \(operationCode)")
        //    }
        //    rawLinks.append(
        //        RawLink(
        //            operationCode: operationCode,
        //            dx: try reader.read(),
        //            dy: try reader.read(),
        //            dz: try reader.read()
        //        )
        //    )
        //} while reader.offset + 16 <= keyframesPackageAddress
        //wadImportLog(rawLinks.last)
        
        
        // Keyframes
        wadImportLog("Jumping into the keyframes block from \(reader.offset) to \(keyframesPackageAddress)")
        reader.set(keyframesPackageAddress)
        let numKeyframesWords: UInt32 = try reader.read()
        let modelsSectionAddress = reader.offset + Int(numKeyframesWords * 2)
        wadImportLog("Number of keyframes words: \(numKeyframesWords) (\(numKeyframesWords * 2) bytes)")
        
        let keyframesBuffer = try reader.readData(ofLength: numKeyframesWords * 2)
        //let keyframesReader = DataReader(with: keyframesBuffer)
        wadImportLog("Read a keyframes buffer of \(keyframesBuffer.count) bytes")
        
        
        // MARK: - Section 5 - Models
        wadImportLog("Jumping into the models block from \(reader.offset) to \(modelsSectionAddress)")
        reader.set(modelsSectionAddress)
        
        // Movables
        let numMovables: UInt32 = try reader.read()
        wadImportLog("Number of movables: \(numMovables)")
        
        struct RawMovable {
            var objectId: TR4ObjectType
            var numPointers: UInt16
            var pointersIndex: UInt16
            
            /// Offset in integers.
            ///
            /// Offset in bytes is:
            ///  ```swift
            ///  linksIndex * 4
            ///  ```
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
        wadImportLog("Number of static objects: \(numStaticObjects)")
        
        struct RawStaticObject {
            //var objectId: UInt32
            var objectId: StaticObjectType
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
            wadImportLog("Successfully reached the end of file 😌")
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
        
        
        // Utilities
        func findMesh(pointersIndex: UInt16) throws -> WKMesh {
            let pointersIndex = Int(pointersIndex)
            guard pointersIndex < meshPointers.count else {
                throw WADError.wrongMeshPointerIndex
            }
            let meshPointer = Int(meshPointers[pointersIndex])
            guard let meshIndex = meshOffsets.firstIndex(of: meshPointer) else {
                throw WADError.wrongMeshOffset
            }
            return wad.meshes[Int(meshIndex)]
        }
        
        func findMeshes(pointersIndex: UInt16, numPointers: UInt16) throws -> [WKMesh] {
            let pointersIndex = Int(pointersIndex)
            let numPointers = Int(numPointers) - 1
            guard pointersIndex + numPointers < meshPointers.count else {
                throw WADError.wrongMeshPointerIndex
            }
            let meshPointer = Int(meshPointers[pointersIndex])
            guard let meshIndex = meshOffsets.firstIndex(of: meshPointer) else {
                throw WADError.wrongMeshOffset
            }
            
            return .init(wad.meshes[Int(meshIndex) ... Int(meshIndex) + numPointers])
        }
        
        
        // Animations
        for rawAnimation in rawAnimations {
            let animation = wad.createAnimation()
            
            if rawAnimation.acceleration > 1 {
                animation.lala()
            }
        }
        
        
        // Movables
        for rawMovable in rawMovables {
            let identifier = rawMovable.objectId
            let meshes = try findMeshes(pointersIndex: rawMovable.pointersIndex, numPointers: rawMovable.numPointers)
            
            let rootJoint: WKJoint? = try {
                let numLinks = Int(rawMovable.numPointers - 1)
                if rawMovable.numPointers < 1 {
                    return nil
                }
                
                var rootJoint = WKJoint(position: .zero, mesh: meshes[0])
                //if rawMovable.objectId == .ANIMATING5 {
                //    rootJoint = rootJoint
                //}
                
                
                var path: [WKJointPath] = [.init(parent: true)]
                
                for i in 0 ..< numLinks {
                    let link = try readLink(at: rawMovable.linksIndex * 4 + UInt32(i * 16))
                    let position = link.position
                    
                    let mesh = meshes[i + 1]
                    
                    switch link.operationCode {
                    case 0:
                        // stack not used. Link the current mesh to the previous mesh.
                        rootJoint.append(mesh, offset: position, at: path)
                        path.append(.init(parent: false))
                        
                    case 1:
                        // pull the parent from the stack. Link the current mesh to the parent.
                        while path.count > 1 && path.last?.parent == false {
                            path.removeLast()
                        }
                        path.removeLast()
                        path.append(.init(parent: false))
                        rootJoint.append(mesh, offset: position, at: path)
                        path.append(.init(parent: false))
                        
                    case 2:
                        // push the parent into the stack. Link the current mesh to the parent.
                        path.removeLast()
                        path.append(.init(parent: true))
                        rootJoint.append(mesh, offset: position, at: path)
                        path.append(.init(parent: false))
                        
                    case 3:
                        // read the parent from the stack. Link the current mesh to the parent.
                        while path.count > 1 && path.last?.parent == false {
                            path.removeLast()
                        }
                        rootJoint.append(mesh, offset: position, at: path)
                        path.append(.init(parent: false))
                        
                    default:
                        throw WADError.unknownJointLinkOperationCode(link.operationCode)
                    }
                }
                
                return rootJoint
            }()
            
            let movable = WKMovable(identifier: identifier, rootJoint: rootJoint)
            wad.movables.append(movable)
        }
        
        
        // Statics
        for rawStaticObject in rawStaticObjects {
            let identifier = rawStaticObject.objectId
            let mesh = try findMesh(pointersIndex: rawStaticObject.pointersIndex)
            let staticObject = WKStaticObject(identifier: identifier, mesh: mesh)
            wad.staticObjects.append(staticObject)
        }
        
        
        return wad
    }
    
    public func serialize() -> Data {
        let dataWriter = DataWriter()
        
        // Version
        dataWriter.write(version.rawValue)
        
        // Texture samples
        dataWriter.write(UInt32(textureSamples.count))
        for textureSample in textureSamples {
            dataWriter.write(textureSample.raw.rawX)
            dataWriter.write(textureSample.raw.rawY)
            dataWriter.write(textureSample.raw.page)
            dataWriter.write(textureSample.raw.flipX)
            dataWriter.write(textureSample.raw.addW)
            dataWriter.write(textureSample.raw.flipY)
            dataWriter.write(textureSample.raw.addH)
        }
        
        // Texture pages
        let pagesSize = texturePages.count * texturePageSize
        dataWriter.write(UInt32(pagesSize))
        for texturePage in texturePages {
            dataWriter.write(texturePage.contents)
        }
        
        // TODO: Construct mesh pointers
        
        return dataWriter.data
    }
}

extension WAD: CustomStringConvertible {
    public var description: String {
        "WAD v\(version). Textures: \(texturePages.count), samples: \(textureSamples.count), meshes: \(meshes.count), animations: \(animations.count)"
    }
}
