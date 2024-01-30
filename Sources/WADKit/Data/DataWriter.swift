//
//  DataWriter.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation


class DataWriter {
    private(set) var data = Data()
    
    func write<SomeType>(_ value: SomeType) where SomeType: BinaryInteger {
        withUnsafePointer(to: value) { pointer in
            data.append(Data(bytes: pointer, count: MemoryLayout<SomeType>.size))
        }
    }
    
    func write<SomeType>(_ value: SomeType) where SomeType: BinaryFloatingPoint {
        withUnsafePointer(to: value) { pointer in
            data.append(Data(bytes: pointer, count: MemoryLayout<SomeType>.size))
        }
    }
    
    func write(_ value: Data) {
        data.append(value)
    }
    
    func skip<SomeType>(_ bytesToSkip: SomeType) where SomeType: BinaryInteger {
        let numBytes = Int(bytesToSkip)
        guard numBytes > 0 else {
            return
        }
        
        data.append(Data(capacity: numBytes))
    }
}


//extension DataWriter {
//    func write(_ value: Float) {
//        write(Float32(value))
//    }
//}
