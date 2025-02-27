//
//  Movable.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


extension DataReader {
    func read() throws -> TR4ObjectType {
        let rawValue: UInt32 = try read()
        guard let value = TR4ObjectType(rawValue: rawValue) else {
            throw DataReaderError.other("Unknown movable identifier: \(rawValue)")
        }
        
        return value
    }
}


public struct Vector3f: Sendable {
    public var x: Float
    public var y: Float
    public var z: Float
    
    public init() {
        x = 0
        y = 0
        z = 0
    }
    
    public init(x: Float, y: Float, z: Float) {
        self.x = x
        self.y = y
        self.z = z
    }
    
    
    public static var zero: Vector3f {
        return Vector3f()
    }
}


struct WKJointPath {
    var parent: Bool
}

public struct WKJoint: Sendable {
    public var position: Vector3f
    public var mesh: WKMesh
    
    public var joints: [WKJoint] = []
    
    mutating func append(_ mesh: WKMesh, offset: Vector3f, at path: any Collection<WKJointPath>) {
        // This should never happen
        if path.isEmpty {
            joints.append(.init(position: offset, mesh: mesh))
        }
        else if path.count == 1 {
            joints.append(.init(position: offset, mesh: mesh))
        }
        else if !joints.isEmpty {
            joints[joints.count - 1].append(mesh, offset: offset, at: path.dropFirst())
        }
        // This should never happen
        else {
            joints.append(.init(position: offset, mesh: mesh))
        }
    }
}


public class WKMovable: @unchecked Sendable {
    private var _identifier: TR4ObjectType
    public var identifier: TR4ObjectType {
        get {
            withWadLock {
                _identifier
            }
        }
        
        set {
            withWadLock {
                _identifier = newValue
            }
        }
    }
    
    
    private(set) public var rootJoint: WKJoint?
    
    
    internal init(identifier: TR4ObjectType, rootJoint: WKJoint?) {
        self._identifier = identifier
        self.rootJoint = rootJoint
    }
}


extension DataReader {
    
}


/*
 
 enum static_types {
 // WAD v129
 PLANT0, PLANT1, PLANT2, PLANT3, PLANT4, PLANT5, PLANT6, PLANT7, PLANT8, PLANT9,
 FURNITURE0, FURNITURE1, FURNITURE2, FURNITURE3, FURNITURE4,
 FURNITURE5, FURNITURE6, FURNITURE7, FURNITURE8, FURNITURE9,
 ROCK0, ROCK1, ROCK2, ROCK3, ROCK4, ROCK5, ROCK6, ROCK7, ROCK8, ROCK9,
 ARCHITECTURE0, ARCHITECTURE1, ARCHITECTURE2, ARCHITECTURE3, ARCHITECTURE4,
 ARCHITECTURE5, ARCHITECTURE6, ARCHITECTURE7, ARCHITECTURE8, ARCHITECTURE9,
 DEBRIS0, DEBRIS1, DEBRIS2, DEBRIS3, DEBRIS4, DEBRIS5, DEBRIS6, DEBRIS7, DEBRIS8, DEBRIS9,
 SHATTER0, SHATTER1, SHATTER2, SHATTER3, SHATTER4, SHATTER5, SHATTER6, SHATTER7, SHATTER8, SHATTER9,
 
 // WAD v130 extension
 EXTRA00, EXTRA01, EXTRA02, EXTRA03, EXTRA04, EXTRA05, EXTRA06, EXTRA07, EXTRA08, EXTRA09,
 EXTRA10, EXTRA11, EXTRA12, EXTRA13, EXTRA14, EXTRA15, EXTRA16, EXTRA17, EXTRA18, EXTRA19,
 EXTRA20, EXTRA21, EXTRA22, EXTRA23, EXTRA24, EXTRA25, EXTRA26, EXTRA27, EXTRA28, EXTRA29,
 EXTRA30, EXTRA31, EXTRA32, EXTRA33, EXTRA34, EXTRA35, EXTRA36, EXTRA37, EXTRA38, EXTRA39,
 EXTRA40, EXTRA41, EXTRA42, EXTRA43, EXTRA44, EXTRA45, EXTRA46, EXTRA47, EXTRA48, EXTRA49,
 EXTRA50, EXTRA51, EXTRA52, EXTRA53, EXTRA54, EXTRA55, EXTRA56, EXTRA57, EXTRA58, EXTRA59,
 EXTRA60, EXTRA61, EXTRA62, EXTRA63, EXTRA64, EXTRA65, EXTRA66, EXTRA67, EXTRA68, EXTRA69,
 EXTRA70, EXTRA71, EXTRA72, EXTRA73, EXTRA74, EXTRA75, EXTRA76, EXTRA77, EXTRA78, EXTRA79,
 EXTRA80, EXTRA81, EXTRA82, EXTRA83, EXTRA84, EXTRA85, EXTRA86, EXTRA87, EXTRA88, EXTRA89,
 EXTRA90, EXTRA91, EXTRA92, EXTRA93, EXTRA94, EXTRA95, EXTRA96, EXTRA97, EXTRA98, EXTRA99,
 
 NUMBER_STATIC_OBJECTS
 };
 
 enum floor_types {
 FLOOR_TYPE, DOOR_TYPE, TILT_TYPE, ROOF_TYPE, TRIGGER_TYPE, LAVA_TYPE, CLIMB_TYPE, SPLIT1, SPLIT2, SPLIT3, SPLIT4,
 NOCOLF1T, NOCOLF1B, NOCOLF2T, NOCOLF2B, NOCOLC1T, NOCOLC1B, NOCOLC2T, NOCOLC2B,
 MONKEY_TYPE, TRIGTRIGGER_TYPE, MINER_TYPE
 };
 
 enum trigger_types {
 TRIGGER, PAD, SWITCH, KEY, PICKUP, HEAVY, ANTIPAD, COMBAT, DUMMY, ANTITRIGGER, HEAVYSWITCH, HEAVYANTITRIGGER, MONKEY
 };
 
 enum trigobj_types {
 TO_OBJECT, TO_CAMERA, TO_SINK, TO_FLIPMAP, TO_FLIPON, TO_FLIPOFF, TO_TARGET, TO_FINISH, TO_CD, TO_FLIPEFFECT, TO_SECRET, TO_BODYBAG, TO_FLYBY, TO_CUTSCENE
 };
 
 */

