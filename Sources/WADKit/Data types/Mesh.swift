//
//  Mesh.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


public struct WKBoundingSphere: Sendable {
    public internal(set) var rawCenterX: Int16
    public internal(set) var rawCenterY: Int16
    public internal(set) var rawCenterZ: Int16
    public internal(set) var rawRadius: UInt16
    public internal(set) var unknown: UInt16
}


public struct WKVector: Sendable {
    public var rawX: Int16
    public var rawY: Int16
    public var rawZ: Int16
    
    public var x: Float { Float(rawX) / 1024.0 }
    public var y: Float { Float(rawY) / 1024.0 }
    public var z: Float { Float(rawZ) / 1024.0 }
    
    public static var zero: WKVector {
        .init(rawX: 0, rawY: 0, rawZ: 0)
    }
    
    public init(rawX: Int16 = 0, rawY: Int16 = 0, rawZ: Int16 = 0) {
        self.rawX = rawX
        self.rawY = rawY
        self.rawZ = rawZ
    }
    
    public func distance(to vector: WKVector) -> Float {
        let dx = x - vector.x
        let dy = y - vector.y
        let dz = z - vector.z
        return sqrt(dx * dx + dy * dy + dz * dz)
    }
    
    public static func + (lhs: WKVector, rhs: WKVector) -> WKVector {
        .init(
            rawX: lhs.rawX + rhs.rawX,
            rawY: lhs.rawY + rhs.rawY,
            rawZ: lhs.rawZ + rhs.rawZ
        )
    }
    
    public static func - (lhs: WKVector, rhs: WKVector) -> WKVector {
        .init(
            rawX: lhs.rawX - rhs.rawX,
            rawY: lhs.rawY - rhs.rawY,
            rawZ: lhs.rawZ - rhs.rawZ
        )
    }
}

extension WKVector: Equatable {
    //
}


/// Quaternion
///
/// [Quaternions](https://www.math.stonybrook.edu/~oleg/courses/mat150-spr16/lecture-5.pdf)
public struct WKQuaternion: Sendable {
    public var ix: Float
    public var iy: Float
    public var iz: Float
    public var r: Float
    
    init(ix: Float, iy: Float, iz: Float, r: Float) {
        self.ix = ix
        self.iy = iy
        self.iz = iz
        self.r = r
    }
}


public struct WKNormal: Sendable {
    public var rawX: Int16
    public var rawY: Int16
    public var rawZ: Int16
    
    public var x: Float { Float(rawX) / 16300 }
    public var y: Float { Float(rawY) / 16300 }
    public var z: Float { Float(rawZ) / 16300 }
    
    
    public func length() -> Float {
        return sqrt(x * x + y * y + z * z)
    }
    
    
    public init(rawX: Int16, rawY: Int16, rawZ: Int16) {
#if false
        self.rawX = rawX
        self.rawY = rawY
        self.rawZ = rawZ
#if DEBUG
        let len = length()
        if len < 0.999 || len > 1.001 {
            rawX = 16300
            print("uh ooooh: \(len) -> (\(x), \(y), \(z))")
        }
#endif
#else
        let tx = Float(rawX) / 16300
        let ty = Float(rawY) / 16300
        let tz = Float(rawZ) / 16300
        let len = sqrt(tx * tx + ty * ty + tz * tz)
        if len < 0.999 || len > 1.001 {
//#if DEBUG
//            print("uh ooooh: \(len) -> (\(tx), \(ty), \(tz))")
//#endif
            if len < 0.0001 {
                self.rawX = 16300
                self.rawY = 0
                self.rawZ = 0
            }
            else {
                self.rawX = Int16(tx / len * 16300)
                self.rawY = Int16(ty / len * 16300)
                self.rawZ = Int16(tz / len * 16300)
            }
        }
        else {
            self.rawX = rawX
            self.rawY = rawY
            self.rawZ = rawZ
        }
#endif
    }
}


public struct WKRotation: Sendable {
    public var rawX: UInt16
    public var rawY: UInt16
    public var rawZ: UInt16
    
    public var x: Float { Float(rawX) / 4096 }
    public var y: Float { Float(rawY) / 4096 }
    public var z: Float { Float(rawZ) / 4096 }
    
    init(rawX: UInt16, rawY: UInt16, rawZ: UInt16) {
        self.rawX = rawX
        self.rawY = rawY
        self.rawZ = rawZ
    }
}


public struct WKShade: Sendable {
    private var _rawValue: Int16 = 0
    private var _value: Float = 0
    
    public var rawValue: Int16 {
        get { _rawValue }
        
        set {
            _rawValue = newValue
            _value = Float(_rawValue) / Float(0x1FFF)
        }
    }
    
    public var value: Float {
        get { _value }
        
        set {
            _value = newValue
            _rawValue = Int16(_value * Float(0x1FFF))
        }
    }
    
    public init() {
        _rawValue = 0x1FFF
        _value = 1
    }
    
    internal init(rawValue: Int16) {
        self.rawValue = rawValue
    }
}


public struct WKPolygon: Sendable {
    struct Triangle: Sendable {
        var v1: UInt16
        var v2: UInt16
        var v3: UInt16
    }
    
    struct Quad: Sendable {
        var v1: UInt16
        var v2: UInt16
        var v3: UInt16
        var v4: UInt16
    }
    
    enum Shape: Sendable {
        case triangle(value: Triangle)
        case quad(value: Quad)
    }
    var shape: Shape
    
    
    enum SampleShape: Sendable {
        case topLeft
        case topRight
        case bottomRight
        case bottomLeft
        case quad
    }
    
    var flippedHorizontally: Bool
    var sampleShape: SampleShape
    var sampleIndex: UInt16
    
    var shineIntensity: UInt8
    var shiny: Bool
    var opaque: Bool
    
    var unknown: UInt8
}

extension DataReader {
    func read() throws -> WKPolygon {
        let shape: WKPolygon.Shape = try {
            let rawShapeType: UInt16 = try read()
            
            switch rawShapeType {
            case 8: return .triangle(
                value: .init(
                    v1: try read(),
                    v2: try read(),
                    v3: try read()
                )
            )
                
            case 9: return .quad(
                value: .init(
                    v1: try read(),
                    v2: try read(),
                    v3: try read(),
                    v4: try read()
                )
            )
                
            default: throw DataReaderError.other("Wrong shape type: \(rawShapeType)")
            }
        }()
        
        let textureFlags: UInt16 = try read()
        
        let flippedHorizontally: Bool = ((textureFlags & 0x8000) >> 15) == 1
        
        let sampleShape: WKPolygon.SampleShape = try {
            let rawSampleShape = ((textureFlags & 0x7000) >> 12)
            switch rawSampleShape {
            case 0: return .topLeft
            case 2: return .topRight
            case 4: return .bottomRight
            case 6: return .bottomLeft
            case 7: return .quad
            default: throw DataReaderError.other("Wrong texture sample shape: \(rawSampleShape)")
            }
        }()
        
        let sampleIndex: UInt16 = {
            if case .triangle(_) = shape {
                return textureFlags & 0x0fff
            }
            
            let index: Int16 = Int16(bitPattern: textureFlags & 0xffff)
            if flippedHorizontally && index < 0 {
                return UInt16(-index)
            }
            
            return UInt16(bitPattern: index)
        }()
        
        let attributes: UInt8 = try read()
        let shineIntensity: UInt8 = (attributes & 0x7c) >> 2
        let shiny: Bool = ((attributes & 0x02) >> 1) == 1
        let opaque: Bool = ((attributes & 0x01)) == 1
        
        let unknown: UInt8 = try read()
        
        return WKPolygon(
            shape: shape,
            flippedHorizontally: flippedHorizontally,
            sampleShape: sampleShape,
            sampleIndex: sampleIndex,
            shineIntensity: shineIntensity,
            shiny: shiny,
            opaque: opaque,
            unknown: unknown
        )
    }
}


public struct WKMesh: Sendable {
    public var boundingSphere: WKBoundingSphere
    public var vertices: [WKVector]
    public var normals: [WKNormal]
    public var shades: [WKShade]
    public var polygons: [WKPolygon]
    
    
    internal init(boundingSphere: WKBoundingSphere, vertices: [WKVector], normals: [WKNormal], shades: [WKShade], polygons: [WKPolygon]) {
        self.boundingSphere = boundingSphere
        self.vertices = vertices
        self.normals = normals
        self.shades = shades
        self.polygons = polygons
    }
}


public struct WKVertexBuffer: Sendable {
    public enum LayoutType: Sendable {
        case normals
        case shades
        case normalsWithWeights
        
        
        public var stride: Int {
            switch self {
            case .normals: 32
            case .shades: 24
            case .normalsWithWeights: 52
            }
        }
    }
    
    public let textureIndex: Int
    public let doubleSided: Bool
    public let layoutType: LayoutType
    public let numVertices: Int
    public let vertexBuffer: Data
    //public let indexBuffer: Data
}


public struct JointConnection: Sendable {
    public let mesh0: Int
    public let offset0: WKVector
    
    public let mesh1: Int
    public let offset1: WKVector
    
    public enum JointType: Sendable {
        /// Vertex indices are hardcoded
        case ponytail
        
        /// Nearest vertices are connected
        case regular
    }
    public let jointType: JointType
    
    public init(mesh0: Int, offset0: WKVector, mesh1: Int, offset1: WKVector, jointType: JointType) {
        self.mesh0 = mesh0
        self.offset0 = offset0
        self.mesh1 = mesh1
        self.offset1 = offset1
        self.jointType = jointType
    }
}


extension WKMesh {
    enum MeshGenerationError: Error {
        case vertexIndexOutOfRange(index: Int)
        case other(_ message: String)
    }
    
    
    // TODO: Also include vertex generation info into WKVertexBuffer for mapping wad vertices into vertex buffer to enable editing the vertices in the wad editor.
    /// Generates a vertex buffer
    ///
    /// If `joint` is specified, then it's a skin joint
    public func generateVertexBuffers(in wad: borrowing WAD, jointInfo joint: JointConnection? = nil, withRemappedTexturePages map: [TexturePageRemapInfo]) async throws -> [WKVertexBuffer] {
        // Layout 1 - 32 stride
        // vx, vy, vz   0   12
        // u, v         12  8
        // nx, ny, nz   20  12
        
        // Layout 2 - 24 stride
        // vx, vy, vz   0   12
        // u, v         12  8
        // shade        20  4
        
        // Layout 3 - 52 stride
        // vx, vy, vz   0   12  // Default position
        // u, v         12  8
        // nx, ny, nz   20  12
        // ox, oy, oz   32  12  // Offset when rendering complete model
        // w0, w1       44  8
        
        
        func getMesh(at index: Int?) -> WKMesh? {
            guard let index else {
                return nil
            }
            
            guard index < wad.meshes.count else {
                return nil
            }
            
            return wad.meshes[index]
        }
        
        //let mesh = getMesh(at: meshIndex)
        let mesh0 = getMesh(at: joint?.mesh0)
        //let offset0 = joint?.offset0
        let mesh1 = getMesh(at: joint?.mesh1)
        let offset1 = joint?.offset1
        
        
        enum UVIndex {
            case v1
            case v2
            case v3
            case v4
        }
        
        struct WeightInfo {
            let offset: WKVector
            let w0: Float
            let w1: Float
        }
        
        struct VertexRemapInfo {
            var vector: WKVector = .zero
            
            var uvIndex: UVIndex = .v1
            var sample: WKTextureSample = .init(raw: .init(rawX: 0, rawY: 0, page: 0, flipX: 0, addW: 0, flipY: 0, addH: 0))
            var u: Int = -1
            var v: Int = -1
            
            var normal: WKNormal? = nil
            
            var shade: Int? = nil
            
            var weightInfo: WeightInfo? = nil
        }
        
        /// Links a buffer with texture
        class BufferBuilder {
            var remapInfo: [VertexRemapInfo] = []
            let writer = DataWriter()
            let indexWriter = DataWriter()
            let opaque: Bool
            let doubleSided: Bool
            let textureIndex: Int
            var numVertices: Int = 0
            
            init(opaque: Bool, doubleSided: Bool, textureIndex: Int) {
                self.opaque = opaque
                self.doubleSided = doubleSided
                self.textureIndex = textureIndex
            }
        }
        // TODO: Also separate with opaque/transparent
        var buffers: [BufferBuilder] = []
        
        func getTextureRemapInfo(forTexturePage pageIndex: UInt16) throws -> TexturePageRemapInfo {
            guard let remapInfo = map.first(where: { $0.pageIndex == pageIndex }) else {
                throw MeshGenerationError.other("Texture page remap info doesn't contain remap info for texture page \(pageIndex)")
            }
            
            return remapInfo
        }
        
        func getBufferBuilder(forTexture textureIndex: Int, opaque: Bool, doubleSided: Bool) -> BufferBuilder {
            if let bufferBuilder = buffers.first(where: { $0.textureIndex == textureIndex }) {
                return bufferBuilder
            }
            
            let bufferBuilder = BufferBuilder(opaque: opaque, doubleSided: doubleSided, textureIndex: textureIndex)
            buffers.append(bufferBuilder)
            return bufferBuilder
        }
        
        
        let lightingType: WKVertexBuffer.LayoutType = {
            guard shades.isEmpty else {
                return .shades
            }
            
            if mesh0 != nil, mesh1 != nil {
                return .normalsWithWeights
            }
            
            return .normals
        }()
        
        
        for polygon in self.polygons {
            guard polygon.sampleIndex < wad.textureSamples.count else {
                throw MeshGenerationError.other("Sample index is out of range \(polygon.sampleIndex) >= \(wad.textureSamples.count)")
            }
            let sample = wad.textureSamples[Int(polygon.sampleIndex)]
            let remapInfo = try getTextureRemapInfo(forTexturePage: sample.raw.page)
            let bufferBuilder = getBufferBuilder(forTexture: remapInfo.textureIndex, opaque: polygon.opaque, doubleSided: true)
            let writer = bufferBuilder.writer
            
            func writeVertex(_ index: UInt16, _ uvIndex: UVIndex) throws {
                let vertexIndex = Int(index)
                guard vertexIndex < vertices.count else {
                    throw MeshGenerationError.other("Vertex index is out of range")
                }
                
                var vertexInfo = VertexRemapInfo()
                
                // Write vertex
                let vertex = vertices[vertexIndex]
                vertexInfo.vector = vertex
                writer.write(vertex.x)
                writer.write(-vertex.y)
                writer.write(-vertex.z)
                
                let rotatedUVIndex: UVIndex = {
                    if case .quad = polygon.shape {
                        return uvIndex
                    }
                    
                    switch polygon.sampleShape {
                    case .topLeft:
                        switch uvIndex {
                        case .v1: return .v1
                        case .v2: return .v2
                        default: return .v4
                        }
                        
                    case .topRight:
                        switch uvIndex {
                        case .v1: return .v2
                        case .v2: return .v3
                        default: return .v1
                        }
                        
                    case .bottomRight:
                        switch uvIndex {
                        case .v1: return .v3
                        case .v2: return .v4
                        default: return .v2
                        }
                        
                    case .bottomLeft:
                        switch uvIndex {
                        case .v1: return .v4
                        case .v2: return .v1
                        default: return .v3
                        }
                        
                    case .quad:
                        return uvIndex
                    }
                }()
                
                // Write UV
                let correctedUVIndex: UVIndex = {
                    if !polygon.flippedHorizontally {
                        return rotatedUVIndex
                    }
                    
                    switch rotatedUVIndex {
                    case .v1: return .v2
                    case .v2: return .v1
                    case .v3: return .v4
                    case .v4: return .v3
                    }
                }()
                vertexInfo.uvIndex = correctedUVIndex
                vertexInfo.sample = sample
                switch correctedUVIndex {
                case .v1:
                    writer.write(sample.raw.left * remapInfo.uvMagnifier + remapInfo.offsetU)
                    writer.write(sample.raw.top * remapInfo.uvMagnifier + remapInfo.offsetV)
                    
                case .v2:
                    writer.write(sample.raw.right * remapInfo.uvMagnifier + remapInfo.offsetU)
                    writer.write(sample.raw.top * remapInfo.uvMagnifier + remapInfo.offsetV)
                    
                case .v3:
                    writer.write(sample.raw.right * remapInfo.uvMagnifier + remapInfo.offsetU)
                    writer.write(sample.raw.bottom * remapInfo.uvMagnifier + remapInfo.offsetV)
                    
                case .v4:
                    writer.write(sample.raw.left * remapInfo.uvMagnifier + remapInfo.offsetU)
                    writer.write(sample.raw.bottom * remapInfo.uvMagnifier + remapInfo.offsetV)
                }
                
                // Write Normal/Shade
                if lightingType == .normals || lightingType == .normalsWithWeights {
                    guard vertexIndex < normals.count else {
                        throw MeshGenerationError.other("Normal index is out of range")
                    }
                    let normal = normals[vertexIndex]
                    vertexInfo.normal = normal
                    writer.write(normal.x)
                    writer.write(-normal.y)
                    writer.write(-normal.z)
                }
                else {
                    guard vertexIndex < shades.count else {
                        throw MeshGenerationError.other("Shade index is out of range")
                    }
                    let shade = shades[vertexIndex]
                    writer.write(shade.value)
                }
                
                
                // Write offset and weight
                if lightingType == .normalsWithWeights {
                    guard let mesh0, !mesh0.vertices.isEmpty/*, let offset0*/, let mesh1, !mesh1.vertices.isEmpty, let offset1 else {
                        throw WADError.other("No mesh info for current joint. This should never happen.")
                    }
                    
                    // TODO: Maybe the best thing to do is to hardcode vertex ids like here?
                    // https://www.tombraiderforums.com/showthread.php?p=2992908
                    
                    let vertex0 = mesh0.vertices.sorted(by: { ($0 - offset1).distance(to: vertex) < ($1 - offset1).distance(to: vertex) })[0] - offset1
                    let vertex1 = mesh1.vertices.sorted(by: { $0.distance(to: vertex) < $1.distance(to: vertex) })[0]
                    
                    let info: WeightInfo = if vertex0.distance(to: vertex) < vertex1.distance(to: vertex) {
                        .init(offset: vertex0 - vertex, w0: 1, w1: 0)
                    }
                    else {
                        .init(offset: vertex1 - vertex, w0: 0, w1: 1)
                    }
                    
                    vertexInfo.weightInfo = info
                    
                    // Offset
                    writer.write(info.offset.x)
                    writer.write(-info.offset.y)
                    writer.write(-info.offset.z)
                    
                    // Weight
                    writer.write(info.w0)
                    writer.write(info.w1)
                }
                
                // TODO: Write vertexInfo and index
                
                bufferBuilder.numVertices += 1
            }
            
            switch polygon.shape {
            case .triangle(let value):
                try writeVertex(value.v1, .v1)
                try writeVertex(value.v3, .v3)
                try writeVertex(value.v2, .v2)
                
            case .quad(let value):
                try writeVertex(value.v1, .v1)
                try writeVertex(value.v3, .v3)
                try writeVertex(value.v2, .v2)
                
                try writeVertex(value.v1, .v1)
                try writeVertex(value.v4, .v4)
                try writeVertex(value.v3, .v3)
            }
        }
        
        return buffers.map {
            WKVertexBuffer(
                textureIndex: $0.textureIndex,
                doubleSided: $0.doubleSided,
                layoutType: lightingType,
                numVertices: $0.numVertices,
                vertexBuffer: $0.writer.data
                //indexBuffer: $0.indexWriter.data
            )
        }
    }
}
