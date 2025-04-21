//
//  WAD.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


public enum WADError: Error, Sendable {
    case notImplemented
    
    case ownerNotFound
    
    case wrongMeshPointerIndex
    case wrongMeshOffset
    
    case unknownJointLinkOperationCode(_ code: Int32)
    case invalidKeyframeBufferSize
    case unknownAxisFormat
    
    case modelNotFound
    
    case other(_ message: String)
}


/// Ta-daaa
public struct WAD: Sendable {
    public private(set) var version: WADVersion = .TombRaiderTheLastRevelation
    public private(set) var texturePages: [WKTexturePage] = []
    public private(set) var textureSamples: [WKTextureSample] = []
    public private(set) var meshes: [WKMesh] = []
    public private(set) var models: [WKModel] = []
    public private(set) var staticObjects: [WKStaticObject] = []
    
    
    public init() {
        //version = .TombRaiderTheLastRevelation
    }
    
    
    public mutating func createTexturePage(withContentsOf data: Data) -> Int {
        let texturePage = WKTexturePage(contents: data)
        texturePages.append(texturePage)
        return texturePages.count
    }
    
    
    public func getSamples(for pageIndex: Int) -> [WKTextureSample] {
        return textureSamples.filter { $0.raw.page == pageIndex }
    }
    
    
    internal mutating func createTextureSample(withRawTextureSample rawSample: RawTextureSample) -> Int {
        let textureSample = WKTextureSample(raw: rawSample)
        textureSamples.append(textureSample)
        return textureSamples.count
    }
    
    
    public func findModel(_ modelIndex: Int) -> WKModel? {
        guard modelIndex >= 0 && modelIndex < models.count else {
            return nil
        }
        
        return models[modelIndex]
    }
    
    
    public func findModel(_ identifier: TR4ObjectType) -> WKModel? {
        models.first { $0.identifier == identifier }
    }
    
    
    public static func fromFileURL(url: URL) async throws -> WAD {
        let data = try Data(contentsOf: url)
        let reader = DataReader(with: data)
        
        var wad = WAD()
        
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
            let boundingSphere = WKBoundingSphere(
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
            
            var vertices: [WKVector] = []
            for _ in 0 ..< numVertives {
                vertices.append(
                    WKVector(
                        rawX: try reader.read(),
                        rawY: try reader.read(),
                        rawZ: try reader.read()
                    )
                )
            }
            
            // Normals/Shades
            let numNormalsOrShades: Int16 = try reader.read()
            //wadImportLog("Number of normals or shades: \(numNormalsOrShades)")
            
            var normals: [WKNormal] = []
            var shades: [WKShade] = []
            
            if numNormalsOrShades > 0 {
                for _ in 0 ..< numNormalsOrShades {
                    normals.append(
                        WKNormal(
                            rawX: try reader.read(),
                            rawY: try reader.read(),
                            rawZ: try reader.read()
                        )
                    )
                }
            }
            else {
                for _ in 0 ..< -numNormalsOrShades {
                    shades.append(WKShade(rawValue: try reader.read()))
                }
            }
            
            // Polygons
            let numPolygons: UInt16 = try reader.read()
            //wadImportLog("Number of polygons: \(numPolygons)")
            var polygons: [WKPolygon] = []
            var numQuads = 0
            for _ in 0 ..< numPolygons {
                let polygon: WKPolygon = try reader.read()
                if case .quad(_) = polygon.shape {
                    numQuads += 1
                }
                polygons.append(polygon)
            }
            
            // Add mesh
            wad.meshes.append(
                .init(
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
            /// Keyframe size in words
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
            
            var position: WKVector {
                return .init(
                    rawX: Int16(dx),
                    rawY: Int16(dy),
                    rawZ: Int16(dz)
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
        
        
        // Keyframes
        wadImportLog("Jumping into the keyframes block from \(reader.offset) to \(keyframesPackageAddress)")
        reader.set(keyframesPackageAddress)
        let numKeyframesWords: UInt32 = try reader.read()
        let modelsSectionAddress = reader.offset + Int(numKeyframesWords * 2)
        wadImportLog("Number of keyframes words: \(numKeyframesWords) (\(numKeyframesWords * 2) bytes)")
        
        let keyframesBuffer = try reader.readData(ofLength: numKeyframesWords * 2)
        wadImportLog("Read a keyframes buffer of \(keyframesBuffer.count) bytes")
        
        struct RawKeyframe {
            var bb1x: Int16
            var bb2x: Int16
            var bb1y: Int16
            var bb2y: Int16
            var bb1z: Int16
            var bb2z: Int16
            var offx: Int16
            var offy: Int16
            var offz: Int16
            var keys: [WKRotation]
        }
        let keyframesReader = DataReader(with: keyframesBuffer)
        func readKeyFrame(numKeys: Int) throws -> RawKeyframe {
            var keyframe = RawKeyframe(
                bb1x: try keyframesReader.read(),
                bb2x: try keyframesReader.read(),
                bb1y: try keyframesReader.read(),
                bb2y: try keyframesReader.read(),
                bb1z: try keyframesReader.read(),
                bb2z: try keyframesReader.read(),
                offx: try keyframesReader.read(),
                offy: try keyframesReader.read(),
                offz: try keyframesReader.read(),
                keys: []
            )
            
            keyframe.keys = try (0 ..< numKeys).map { _ in
                let word: UInt16 = try keyframesReader.read()
                let axes = word & 0xC000
                switch axes {
                case 0x0000:
                    let secondWord: UInt16 = try keyframesReader.read()
                    let rotation: UInt32 = (UInt32(word) << 16) | UInt32(secondWord)
                    let rotationX = UInt16(((0x3FF << 20) & rotation) >> 20)
                    let rotationY = UInt16(((0x3FF << 10) & rotation) >> 10)
                    let rotationZ = UInt16(((0x3FF <<  0) & rotation) >> 0)
                    return WKRotation(
                        rawX: rotationX * 4,
                        rawY: rotationY * 4,
                        rawZ: rotationZ * 4
                    )
                    
                case 0x4000:
                    return WKRotation(rawX: word & 0x3FFF, rawY: 0, rawZ: 0)
                    
                case 0x8000:
                    return WKRotation(rawX: 0, rawY: word & 0x3FFF, rawZ: 0)
                    
                case 0xC000:
                    return WKRotation(rawX: 0, rawY: 0, rawZ: word & 0x3FFF)
                    
                default:
                    throw WADError.unknownAxisFormat
                }
            }
            
            return keyframe
        }
        
        
        // MARK: - Section 5 - Models
        wadImportLog("Jumping into the models block from \(reader.offset) to \(modelsSectionAddress)")
        reader.set(modelsSectionAddress)
        
        // Movables
        let numMovables: UInt32 = try reader.read()
        wadImportLog("Number of models: \(numMovables)")
        
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
        func findMesh(pointersIndex: UInt16) throws -> Int {
            let pointersIndex = Int(pointersIndex)
            guard pointersIndex < meshPointers.count else {
                throw WADError.wrongMeshPointerIndex
            }
            let meshPointer = Int(meshPointers[pointersIndex])
            guard let meshIndex = meshOffsets.firstIndex(of: meshPointer) else {
                throw WADError.wrongMeshOffset
            }
            return Int(meshIndex)
        }
        
        func findMeshes(pointersIndex: UInt16, numPointers: UInt16) throws -> [Int] {
            //if pointersIndex % 2 > 0 {
            //    print("uh-ooh")
            //}
            let pointersIndex = Int(pointersIndex)
            let numPointers = Int(numPointers) - 1
            guard pointersIndex + numPointers < meshPointers.count else {
                throw WADError.wrongMeshPointerIndex
            }
            let meshPointer = Int(meshPointers[pointersIndex])
            guard let meshIndex = meshOffsets.firstIndex(of: meshPointer) else {
                throw WADError.wrongMeshOffset
            }
            
            return .init(Int(meshIndex) ... Int(meshIndex) + numPointers)
        }
        
        
        // Movables
        for (modelIndex, rawMovable) in rawMovables.enumerated() {
            let identifier = rawMovable.objectId
            if rawMovable.objectId == .MUMMY {
                print("a-ha")
            }
            let meshes = try findMeshes(pointersIndex: rawMovable.pointersIndex, numPointers: rawMovable.numPointers)
            
            // Joints
            let rootJoint: WKJoint? = try {
                let numLinks = Int(rawMovable.numPointers - 1)
                if rawMovable.numPointers < 1 {
                    return nil
                }
                
                var rootJoint = WKJoint(offset: .zero, mesh: meshes[0])
                
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
            
            
            // Number of animations for current model
            let startRawAnimationIndex = Int(rawMovable.animsIndex)
            let numAnimations = {
                // Current model does not have any animation
                if rawMovable.animsIndex < 0 {
                    return 0
                }
                
                // The nearest next model has animations
                var nextIndex = modelIndex + 1
                while nextIndex < rawMovables.count - 1 {
                    let nextRawModel = rawMovables[nextIndex]
                    if nextRawModel.animsIndex >= 0 {
                        return Int(nextRawModel.animsIndex - rawMovable.animsIndex)
                    }
                    
                    nextIndex += 1
                }
                
                // Current model is the last model with animations
                return rawAnimations.count - Int(rawMovable.animsIndex)
            }()
            
            
            // Animations
            //rawMovable.animsIndex
            let numMeshes = meshes.count
            var animations: [WKAnimation] = []
            for rawAnimationIndex in startRawAnimationIndex ..< startRawAnimationIndex + numAnimations {
                let rawAnimation = rawAnimations[rawAnimationIndex]
                let keyframeSize = Int(rawAnimation.keyframeSize * 2)
                
                let numKeyframes: Int = try {
                    guard keyframeSize > 0 else {
                        return 0
                    }
                    
                    let nextRawAnimation: RawAnimation? = {
                        guard rawAnimationIndex < rawAnimations.count - 1 else {
                            return nil
                        }
                        
                        return rawAnimations[rawAnimationIndex + 1]
                    }()
                    
                    let nextOffset = {
                        guard let nextRawAnimation else {
                            return keyframesBuffer.count
                        }
                        
                        return Int(nextRawAnimation.keyframeOffset)
                    }()
                    
                    let offsetDelta = nextOffset - Int(rawAnimation.keyframeOffset)
                    guard offsetDelta % keyframeSize == 0 else {
                        throw WADError.invalidKeyframeBufferSize
                    }
                    
                    return offsetDelta / keyframeSize
                }()
                
                //rawAnimation.nextAnimation
                let animation = try WKAnimation(
                    frameDuration: Int(rawAnimation.frameDuration),
                    nextAnimation: Int(rawAnimation.nextAnimation) - startRawAnimationIndex,
                    nextAnimationFrameIn: Int(rawAnimation.frameIn),
                    keyframes: (0 ..< numKeyframes).map { idx in
                        keyframesReader.set(Int(rawAnimation.keyframeOffset) + keyframeSize * idx)
                        let rawKeyframe = try readKeyFrame(numKeys: numMeshes)
                        let offset = WKVector(rawX: rawKeyframe.offx, rawY: rawKeyframe.offy, rawZ: rawKeyframe.offz)
                        return .init(offset: offset, rotations: rawKeyframe.keys)
                    }
                )
                animations.append(animation)
            }
            
            let movable = WKModel(identifier: identifier, rootJoint: rootJoint, animations: animations)
            wad.models.append(movable)
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
        "WAD v\(version). Textures: \(texturePages.count), samples: \(textureSamples.count), meshes: \(meshes.count)"
    }
}
