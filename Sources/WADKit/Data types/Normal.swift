//
//  Normal.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


struct WADNormal {
    private struct NormalValue {
        private var _raw: Int16
        var raw: Int16 {
            get { _raw }
            set {
                _raw = newValue
                _value = Float(newValue) / 1024
            }
        }
        
        private var _value: Float
        var value: Float {
            get { _value }
            set {
                _value = newValue
                _raw = Int16(newValue * 16300)
            }
        }
        
        init(_ raw: Int16) {
            self._raw = 0
            self._value = 0
            self.raw = raw
        }
        
        init(_ value: Float) {
            self._raw = 0
            self._value = 0
            self.value = value
        }
    }
    
    private var internalX: NormalValue
    private var internalY: NormalValue
    private var internalZ: NormalValue
    
    var rawX: Int16 {
        get { internalX.raw }
        set { internalX.raw = newValue }
    }
    var rawY: Int16 {
        get { internalY.raw }
        set { internalY.raw = newValue }
    }
    var rawZ: Int16 {
        get { internalZ.raw }
        set { internalZ.raw = newValue }
    }
    
    
    var x: Float {
        get { internalZ.value }
        set { internalZ.value = newValue }
    }
    var y: Float {
        get { internalZ.value }
        set { internalZ.value = newValue }
    }
    var z: Float {
        get { internalZ.value }
        set { internalZ.value = newValue }
    }
    
    
    init(x: Int16, y: Int16, z: Int16) {
        self.internalX = .init(x)
        self.internalY = .init(y)
        self.internalZ = .init(z)
    }
    
    init(x: Float, y: Float, z: Float) {
        self.internalX = .init(x)
        self.internalY = .init(y)
        self.internalZ = .init(z)
    }
}

