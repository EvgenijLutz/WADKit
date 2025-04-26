//
//  Math.swift
//  WADKit
//
//  Created by Evgenij Lutz on 26.04.25.
//

import Foundation


public struct MTVector3: Sendable {
    public var x: Float
    public var y: Float
    public var z: Float
    
    
    public init(_ x: Float, _ y: Float, _ z: Float) {
        self.x = x
        self.y = y
        self.z = z
    }
    
    
    public init(x: Float, y: Float, z: Float) {
        self.x = x
        self.y = y
        self.z = z
    }
    
    
    
}

public func * (lhs: MTVector3, rhs: Float) -> MTVector3 {
    .init(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs)
}


/// Quaternion
///
/// [Quaternions](https://www.math.stonybrook.edu/~oleg/courses/mat150-spr16/lecture-5.pdf)
public struct MTQuaternion: Sendable {
    public var ix: Float
    public var iy: Float
    public var iz: Float
    public var r: Float
    
    public init(ix: Float, iy: Float, iz: Float, r: Float) {
        self.ix = ix
        self.iy = iy
        self.iz = iz
        self.r = r
    }
    
    public static func fromEuler(_ euler: MTVector3) -> MTQuaternion {
        let cx = cos(euler.x / 2)
        let cy = cos(euler.y / 2)
        let cz = cos(euler.z / 2)
        let sx = sin(euler.x / 2)
        let sy = sin(euler.y / 2)
        let sz = sin(euler.z / 2)
        
        let w = cx * cy * cz + sx * sy * sz
        let x = sx * cy * cz - cx * sy * sz
        let y = cx * sy * cz + sx * cy * sz
        let z = cx * cy * sz - sx * sy * cz
        
        return .init(ix: x, iy: y, iz: z, r: w);
    }
}


public func * (lhs: MTQuaternion, rhs: MTQuaternion) -> MTQuaternion {
    let ix = lhs.r * rhs.ix + lhs.ix * rhs.r  + lhs.iy * rhs.iz - lhs.iz * rhs.iy
    let iy = lhs.r * rhs.iy - lhs.ix * rhs.iz + lhs.iy * rhs.r  + lhs.iz * rhs.ix
    let iz = lhs.r * rhs.iz + lhs.ix * rhs.iy - lhs.iy * rhs.ix + lhs.iz * rhs.r
    let r  = lhs.r * rhs.r  - lhs.ix * rhs.ix - lhs.iy * rhs.iy - lhs.iz * rhs.iz
    return .init(ix: ix, iy: iy, iz: iz, r: r)
}
