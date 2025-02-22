//
//  Version.swift
//  WADKit
//
//  Created by Evgenij Lutz on 27.01.24.
//

import Foundation


public enum WADVersion: UInt32, Sendable {
    case TombRaiderTheLastRevelation = 129
    case TombRaiderAbandonedNextGeneration = 130
}


extension WADVersion: CustomStringConvertible {
    public var description: String {
        let name: String = {
            switch self {
            case .TombRaiderTheLastRevelation: "Tomb Raider: The Last Revelation"
            case .TombRaiderAbandonedNextGeneration: "Abandoned \"Next Generation\""
            }
        }()
        
        return "\(self.rawValue) - \(name)"
    }
}


extension DataReader {
    func read() throws -> WADVersion {
        let rawVersion: UInt32 = try read()
        guard let version = WADVersion(rawValue: rawVersion) else {
            throw WADError.other("Wrong WAD version number: \(rawVersion)")
        }
        
        return version
    }
}
