//
//  StaticObject.swift
//  WADKit
//
//  Created by Evgenij Lutz on 25.02.25.
//

import Foundation


extension DataReader {
    func read() throws -> StaticObjectType {
        let rawValue: UInt32 = try read()
        guard let value = StaticObjectType(rawValue: rawValue) else {
            throw DataReaderError.other("Unknown static object identifier: \(rawValue)")
        }
        
        return value
    }
}


public class WKStaticObject: @unchecked Sendable {
    private var _identifier: StaticObjectType
    public var identifier: StaticObjectType {
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
    
    private var _mesh: WKMesh
    public var mesh: WKMesh {
        get {
            withWadLock {
                _mesh
            }
        }
        
        set {
            withWadLock {
                _mesh = newValue
            }
        }
    }
    
    init(identifier: StaticObjectType, mesh: WKMesh) {
        self._identifier = identifier
        self._mesh = mesh
    }
}
