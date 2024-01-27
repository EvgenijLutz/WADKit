//
//  Mesh.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation


public struct BoundingSphere {
    public internal(set) var rawCenterX: Int16
    public internal(set) var rawCenterY: Int16
    public internal(set) var rawCenterZ: Int16
    public internal(set) var rawRadius: UInt16
    public internal(set) var unknown: UInt16
}


public struct Vertex {
    public var rawX: Int16
    public var rawY: Int16
    public var rawZ: Int16
    
    public var x: Float { Float(rawX) / 1024 }
    public var y: Float { Float(rawY) / 1024 }
    public var z: Float { Float(rawZ) / 1024 }
}


public struct Normal {
    public var rawX: Int16
    public var rawY: Int16
    public var rawZ: Int16
    
    public var x: Float { Float(rawX) / 16300 }
    public var y: Float { Float(rawY) / 16300 }
    public var z: Float { Float(rawZ) / 16300 }
}


public struct Shade {
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


public struct Polygon {
    struct Triangle {
        var v1: UInt16
        var v2: UInt16
        var v3: UInt16
    }
    
    struct Quad {
        var v1: UInt16
        var v2: UInt16
        var v3: UInt16
        var v4: UInt16
    }
    
    enum Shape {
        case triangle(value: Triangle)
        case quad(value: Quad)
    }
    var shape: Shape
    
    
    enum SampleShape {
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
    func read() throws -> Polygon {
        let shape: Polygon.Shape = try {
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
        
        let sampleShape: Polygon.SampleShape = try {
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
        
        let sampleIndex: UInt16 = textureFlags & 0x0fff
        
        let attributes: UInt8 = try read()
        let shineIntensity: UInt8 = (attributes & 0x7c) >> 2
        let shiny: Bool = ((attributes & 0x02) >> 1) == 1
        let opaque: Bool = ((attributes & 0x01)) == 1
        
        let unknown: UInt8 = try read()
        
        return Polygon(
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


public class Mesh {
    private (set) weak var owner: WAD?
    
    public var boundingSphere: BoundingSphere
    public var vertices: [Vertex]
    public var normals: [Normal]
    public var shades: [Shade]
    public var polygons: [Polygon]
    
    
    internal init(owner: WAD, boundingSphere: BoundingSphere, vertices: [Vertex], normals: [Normal], shades: [Shade], polygons: [Polygon]) {
        self.owner = owner
        self.boundingSphere = boundingSphere
        self.vertices = vertices
        self.normals = normals
        self.shades = shades
        self.polygons = polygons
    }
}
