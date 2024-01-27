//
//  DataReader.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation


class DataReader {
    enum DataReaderError: Error {
        case strangeDataSize(offset: Int, type: Any.Type)
        case indexOutOfRange(offset: Int, endOffset: Int, type: Any.Type)
        case unwrapMemoryError(offset: Int, endOffset: Int, type: Any.Type)
        case other(_ message: String)
    }
    
    let data: Data
    private(set) var offset: Int
    
    
    init(with data: Data) {
        self.data = data
        self.offset = 0
    }
    
    func reset() {
        offset = 0
    }
    
    func set<SomeType>(_ newOffset: SomeType) where SomeType: BinaryInteger {
        offset = Int(newOffset)
    }
    
    func skip<SomeType>(_ bytesToSkip: SomeType) where SomeType: BinaryInteger {
        let numBytes = Int(bytesToSkip)
        offset += numBytes
    }
    
    func back(_ bytesToGoBack: Int) {
        offset -= bytesToGoBack
    }
    
    func read<SomeType>() throws -> SomeType {
        let length = MemoryLayout<SomeType>.size
        guard length > 0 else {
            throw DataReaderError.strangeDataSize(offset: offset, type: SomeType.self)
        }
        
        let endIndex = offset + length
        guard offset >= 0 && endIndex <= data.count else {
            throw DataReaderError.indexOutOfRange(offset: offset, endOffset: endIndex, type: SomeType.self)
        }
        
        let value = try data[offset ..< endIndex].withUnsafeBytes {
            // Method 1 - Will crash if memory is not properly aligned
            //return $0.load(as: SomeType.self)
            
            // Method 2 - Also will crash if memory is not properly aligned
            //guard let baseAddress = $0.baseAddress else {
            //    throw DataReaderError.UnwrapMemoryError(offset: offset, endOffset: endIndex, type: SomeType.self)
            //}
            //
            //return baseAddress.withMemoryRebound(to: SomeType.self, capacity: 1) {
            //    $0.pointee
            //}
            
            // Method 3 - Will not crash because it's able to read from unaligned memory, but the data type must be trivial like UInt32 or Int8 and other
            guard let value = $0.baseAddress?.loadUnaligned(as: SomeType.self) else {
                throw DataReaderError.unwrapMemoryError(offset: offset, endOffset: endIndex, type: SomeType.self)
            }
            
            return value
        }
        
        offset = endIndex
        return value
    }
    
    func readData<Integer>(ofLength length: Integer) throws -> Data where Integer: BinaryInteger {
        guard length > 0 else {
            throw DataReaderError.strangeDataSize(offset: offset, type: Data.self)
        }
        
        let endIndex = offset + Int(length)
        guard offset >= 0 && endIndex <= data.count else {
            throw DataReaderError.indexOutOfRange(offset: offset, endOffset: endIndex, type: Data.self)
        }
        
        let value = data[offset ..< endIndex]
        
        offset = endIndex
        return value
    }
}
