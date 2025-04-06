//
//  DataWriter.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


class DataWriter {
    private(set) var data = Data()
    
    func write<SomeType: BinaryInteger>(_ value: SomeType) {
        withUnsafePointer(to: value) { pointer in
            data.append(Data(bytes: pointer, count: MemoryLayout<SomeType>.size))
        }
    }
    
    func write<SomeType: BinaryFloatingPoint>(_ value: SomeType) {
        withUnsafePointer(to: value) { pointer in
            data.append(Data(bytes: pointer, count: MemoryLayout<SomeType>.size))
        }
    }
    
    func write(_ value: Data) {
        data.append(value)
    }
    
    func skip<SomeType: BinaryInteger>(_ bytesToSkip: SomeType) {
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
