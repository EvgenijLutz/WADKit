//
//  WAD2.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.04.25.
//

import Foundation
import DataIO

#if os(macOS) || os(iOS)
import zlib
#else
// import zlib port on other platforms
#endif


// MARK: WAD2

public enum WAD2PolygonContents: Sendable {
    case quad(v0: Int, v1: Int, v2: Int, v3: Int)
    case triangle(v0: Int, v1: Int, v2: Int)
}

public struct WAD2Polygon: Sendable {
    var contents: WAD2PolygonContents
}


public struct WAD2Mesh: Sendable {
    var obsoleteMeshIndex: Int? = 0
    var name: String? = nil
}


public struct WAD2Movable: Sendable {
    var objectId: Int = 0
    var meshes: [WAD2Mesh] = []
}


public struct WAD2: Sendable {
    public var movables: [WAD2Movable] = []
    
    public var customChunks: [WAD2Chunk] = []
}


public typealias WAD2ChunkID = String


public extension WAD2ChunkID {
    static let GameVersion = "W2SuggestedGameVersion"
    static let SoundSystem = "W2SoundSystem"
    static let Metadata = "W2Metadata"
    static let Timestamp = "W2Timestamp"
    static let UserNotes = "W2UserNotes"
    //static let TrNgWadObsolete = "W2TrNgWad"
    //static let SoundManagementSystemObsolete = "W2SoundMgmt"
    static let Textures = "W2Textures"
    static let Texture = "W2Txt"
    static let TextureIndex = "W2Index"
    static let TextureData = "W2TxtData"
    static let TextureName = "W2TxtName"
    static let Samples = "W2WaveSamples"
    static let Sample = "W2Wav"
    static let SampleIndex = "W2Index"
    static let SampleFilenameObsolete = "W2WavName"
    static let SampleData = "W2WavData"
    static let SoundInfosObsolete = "W2Sounds"
    static let SoundInfos = "W2Sounds2"
    static let SoundInfo = "W2Sound"
    static let SoundInfoNameObsolete = "W2SampleName"
    static let SoundInfoName = "W2Name"
    static let SoundInfoIndex = "W2Index"
    static let SoundInfoVolume = "W2Vol"
    static let SoundInfoRange = "W2Ran"
    static let SoundInfoPitch = "W2Pit"
    static let SoundInfoChance = "W2Cha"
    static let SoundInfoDisablePanning = "W2NoPan"
    static let SoundInfoRandomizePitch = "W2RngP"
    static let SoundInfoRandomizeVolume = "W2RngV"
    static let SoundInfoLoopBehaviour = "W2Loop"
    static let SoundInfoTargetSampleRate = "W2SamplRate"
    static let SoundInfoSampleIndex = "W2SamplePtr"
    static let SoundInfoSampleName = "W2SampleName"
    static let FixedSoundInfos = "W2FixedSounds"
    static let FixedSoundInfo = "W2FixedSound"
    static let FixedSoundInfoId = "W2Id"
    static let FixedSoundInfoSoundInfoId = "W2Sound"
    static let AdditionalSoundInfosObsolete = "W2Sounds3"
    static let AdditionalSoundInfos = "W2AdditionalSoundInfos"
    static let AdditionalSoundInfo = "W2AdditionalSoundInfo"
    static let AdditionalSoundInfoSoundInfoId = "W2Id"
    static let AdditionalSoundInfoName = "W2Name"
    static let Meshes = "W2Meshes"
    static let Mesh = "W2Mesh"
    static let MeshIndex = "W2Index"
    static let MeshName = "W2MeshName"
    static let MeshSphere = "W2MeshSphere"
    static let MeshSphereCenter = "W2SphC"
    static let MeshSphereRadius = "W2SphR"
    static let MeshBoundingBox = "W2BBox"
    static let MeshBoundingBoxMin = "W2BBMin"
    static let MeshBoundingBoxMax = "W2BBMax"
    static let MeshVertexPositions = "W2VrtPos"
    static let MeshVertexPosition = "W2Pos"
    static let MeshVertexNormals = "W2VrtNorm"
    static let MeshVertexNormal = "W2N"
    static let MeshVertexShades = "W2VrtShd"
    static let MeshVertexShade = "W2Shd"
    static let MeshVertexColor = "W2VxCol"
    static let MeshVertexAttributes = "W2VrtAttr"
    static let MeshVertexAttribute = "W2VxAttr"
    static let MeshLightingType = "W2MeshLightType"
    static let MeshPolygons = "W2Polys"
    static let MeshTriangle = "W2Tr"
    static let MeshQuad = "W2Uq"
    static let MeshTriangle2 = "W2Tr2"
    static let MeshQuad2 = "W2Uq2"
    static let MeshPolygonExtraObsolete = "W2Pe"
    static let MeshPolygonIndices = "W2PolyInd"
    static let MeshPolygonIndex = "W2Ind"
    static let MeshPolygonTexCoords = "W2PolyUV"
    static let MeshPolygonTexCoord = "W2UV"
    static let Sprites = "W2Sprites"
    static let Sprite = "W2Spr"
    static let SpriteIndex = "W2Index"
    static let SpriteSides = "W2SprSides"
    static let SpriteData = "W2TxtData"
    static let SpriteSequences = "W2SpriteSequences"
    static let SpriteSequence = "W2SpriteSeq"
    static let SpriteSequenceName = "W2SpriteSeqName"
    static let SpriteSequenceData = "W2SpriteSeqData"
    static let SpriteSequenceSpriteIndices = "W2SpriteSeqSprites"
    static let SpriteSequenceSpriteIndex = "W2SpritePtr"
    static let Moveables = "W2Moveables"
    static let Moveable = "W2Moveable"
    //static let MoveableName = "W2MovName"
    static let MoveableMeshes = "W2MovMeshes"
    static let MoveableMesh = "W2MovMeshPtr"
    static let MoveableLinks = "W2Links"
    static let MoveableLink = "W2Lnk"
    static let MoveableBone = "W2Bone"
    static let MoveableBoneNew = "W2Bone2"
    static let MoveableBoneTransform = "W2BoneTransf"
    static let MoveableBoneMeshPointer = "W2BoneMesh"
    static let MoveableBoneName = "W2BoneName"
    static let MoveableBoneTranslation = "W2BoneTrans"
    static let MoveableLinkOffset = "W2LnkO"
    static let Animations = "W2Animations"
    static let AnimationObsolete = "W2Anm"
    static let Animation = "W2Ani"
    static let Animation2 = "W2Ani2"
    static let AnimationVelocities = "W2AniV"
    static let AnimationName = "W2AnmName"
    static let StateChanges = "W2StChs"
    static let StateChange = "W2StCh"
    static let Dispatches = "W2Disps"
    static let Dispatch = "W2Disp"
    static let KeyFrames = "W2Kfs"
    static let KeyFrame = "W2Kf"
    static let KeyFrameBoundingBox = "W2KfBB"
    static let KeyFrameOffset = "W2KfOffs"
    static let KeyFrameAngles = "W2KfAs"
    static let KeyFrameAngle = "W2KfA"
    static let AnimCommands = "W2Cmds"
    static let AnimCommand = "W2Cmd"
    static let AnimCommand2 = "W2Cmd2"
    /// Deprecated
    static let AnimCommandSoundInfo = "W2CmdSnd"
    static let Statics = "W2Statics"
    static let Static = "W2Static"
    static let Static2 = "W2Static2"
    static let StaticVisibilityBox = "W2StaticVB"
    static let StaticCollisionBox = "W2StaticCB"
    static let StaticAmbientLight = "W2StaticAmbientLight"
    static let StaticLight = "W2StaticLight"
    static let StaticLightPosition = "W2StaticLightPos"
    static let StaticLightRadius = "W2StaticLightR"
    static let StaticLightIntensity = "W2StaticLightI"
    static let StaticShatter = "W2StaticShatter"
    static let StaticShatterSound = "W2StaticShatterSound"
}


public struct WAD2Chunk: Sendable {
    public let id: WAD2ChunkID
    public let data: Data
}



struct LEB128Long {
    var value: Int
}


extension DataReader {
    mutating func read() throws -> LEB128Long {
        var result: Int = 0
        var currentShift: Int = 0
        
        var currentByte: UInt8 = 0
        repeat {
            currentByte = try read()
            result |= Int(currentByte & 0x7F) << currentShift
            currentShift += 7
            
            //guard currentShift < 64 else {
            //    throw WADError.other("LEB128Long overflow")
            //}
        } while currentByte & 0x80 != 0
        
        let shift = 64 - currentShift
        if shift > 0 {
            result = (result << shift) >> shift
        }
        
        return .init(value: result)
    }
    
    
    mutating func read() throws -> WAD2Chunk {
        let chunkIdLength: LEB128Long = try read()
        let chunkIdData = try readData(ofLength: chunkIdLength.value)
        guard let chunkId = String(data: chunkIdData, encoding: .utf8) else {
            throw WADError.other("Cannot read chunk ID")
        }
        
        guard chunkIdLength.value > 0 else {
            return .init(id: chunkId, data: .init())
        }
        
        let chunkLength: LEB128Long = try read()
        let chunkData = try readData(ofLength: chunkLength.value)
        
        return .init(id: chunkId, data: chunkData)
    }
    
    
    mutating func readChunks(action: (_ chunk: WAD2Chunk) throws -> Void) throws {
        var safetyCounter = 0
        repeat {
            let chunk: WAD2Chunk = try read()
            if chunk.id == "" {
                //print("End of chunk")
                break
            }
            //print("Chunk ID: \(chunk.id)")
            
            try action(chunk)
            
            safetyCounter += 1
            if safetyCounter > 1000000 {
                throw WADError.other("Too many iterations performed while reading wad2 chunks")
            }
        } while(offset < data.count)
    }
}


// MARK: Load WAD2

func oin<DataType: Equatable>(_ value: DataType, _ array: Array<DataType>) -> Bool {
    return array.contains(value)
}


func oin<DataType: Equatable>(_ value: DataType, _ array: DataType...) -> Bool {
    return array.contains(value)
}


public extension WAD2 {
    static func fromFile(at url: URL) async throws -> WAD2 {
        let data = try Data(contentsOf: url)
        var reader = DataReader(data)
        
        // WAD2 magic
        let wad2MagicObsolete = UInt32(0x32_64_61_57)
        let wad2Magic = UInt32(0x32_44_41_57)
        let magic: UInt32 = try reader.read()
        print("Read magic: \(String(format: "%08X", magic))")
        if magic == wad2MagicObsolete {
            throw WADError.other("Loaded wad2 version is deprecated and not supported. Please use version 1.3.15 or earlier.")
        }
        
        guard magic == wad2Magic else {
            throw WADError.other("Unknown wad2 version: \(String(format: "%08X", magic))")
        }
        
        
        // Chunk version
        let chunkVersion: UInt32 = try reader.read()
        if chunkVersion & 0x7fffffff != 0 {
            throw WADError.other("Unknown chunk version: \(String(format: "%08X", chunkVersion))")
        }
        
        
        // Data compression
        if chunkVersion & 0x80000000 != 0 {
            print("Compressed")
            throw WADError.notImplemented
        }
        else {
            print("Uncompressed")
        }
        
        
        // Read chunks
        var wad = WAD2()
        try reader.readChunks { chunk in
            switch chunk.id {
            case "W2Moveables":
                try wad.importMovables(from: chunk.data)
                
            default:
                break
            }
        }
        
        
        
        return wad
    }
    
    
    mutating func importMovables(from data: Data) throws {
        var reader = DataReader(data)
        try reader.readChunks { chunk in
            guard chunk.id == "W2Moveable" else {
                return
            }
            
            var movable = WAD2Movable()
            
            var reader = DataReader(chunk.data)
            let objectId: LEB128Long = try reader.read()
            movable.objectId = objectId.value
            
            try reader.readChunks { chunk in
                switch chunk.id {
                case "W2Mesh":
                    var mesh = WAD2Mesh()
                    var reader = DataReader(chunk.data)
                    try reader.readChunks { chunk in
                        switch chunk.id {
                        case "W2Index":
                            var reader = DataReader(chunk.data)
                            let index: LEB128Long = try reader.read()
                            mesh.obsoleteMeshIndex = index.value
                            break
                            
                        case "W2MeshName":
                            if let name = String(data: chunk.data, encoding: .utf8) {
                                mesh.name = name
                            }
                            break
                            
                        case "W2MeshSphere":
                            break
                            
                        case "W2BBox":
                            break
                            
                        case "W2VrtPos":
                            break
                            
                        case "W2VrtNorm":
                            break
                            
                        case "W2VrtShd":
                            break
                            
                        case .MeshVertexAttribute:
                            break
                            
                        case .MeshLightingType:
                            break
                            
                        case .MeshPolygons:
                            var reader = DataReader(chunk.data)
                            try reader.readChunks { chunk in
                                let isQuad = oin(chunk.id, .MeshQuad, .MeshQuad2)
                                let isTriangle = oin(chunk.id, .MeshTriangle, .MeshTriangle2)
                                guard isQuad || isTriangle else {
                                    throw WADError.other("Unknown polygon type: \(chunk.id)")
                                }
                                
                                var reader = DataReader(chunk.data)
                                let v0: LEB128Long = try reader.read()
                                let v1: LEB128Long = try reader.read()
                                let v2: LEB128Long = try reader.read()
                                let contents: WAD2PolygonContents = try {
                                    if isQuad {
                                        let v3: LEB128Long = try reader.read()
                                        return .quad(v0: v0.value, v1: v1.value, v2: v2.value, v3: v3.value)
                                    }
                                    else {
                                        return .triangle(v0: v0.value, v1: v1.value, v2: v2.value)
                                    }
                                }()
                                
                                //print(contents)
                            }
                            break
                            
                        default:
                            break
                        }
                    }
                    
                    movable.meshes.append(mesh)
                    break
                    
                case "W2Bone":
                    break
                    
                case "W2Bone2":
                    break
                    
                case .Animation2:
                    break
                    
                default:
                    print("Unknown chunk type: \(chunk.id)")
                    break
                }
            }
            
            movables.append(movable)
        }
    }
    
}
