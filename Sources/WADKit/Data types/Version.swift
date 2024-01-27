//
//  Version.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation


public enum Version: UInt32 {
    case TombRaiderTheLastRevelation = 129
    case TombRaiderAbandonedNextGeneration = 130
}


extension Version: CustomStringConvertible {
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
    func read() throws -> Version {
        let rawVersion: UInt32 = try read()
        guard let version = Version(rawValue: rawVersion) else {
            throw WAD.LoadError.custom("Wrong WAD version number: \(rawVersion)")
        }
        
        return version
    }
}
