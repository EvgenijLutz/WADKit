//
//  Logging.swift
//  WADKit
//
//  Created by Eugen Lutz on 27.01.24.
//

import Foundation

#if os(macOS) || os(iOS) || os(tvOS)

import OSLog

@available(macOS 11.0, *)
fileprivate class WADOSLogger: WADLogger {
    internal let logger = Logger(subsystem: "WADKit", category: "log")
    
    func log(_ message: String) {
        logger.log("\(message)")
    }
}

#endif


fileprivate protocol WADLogger {
    func log(_ message: String)
}


fileprivate class WADPrintLogger: WADLogger {
    func log(_ message: String) {
        print(message)
    }
}

fileprivate let logger: WADLogger = {
#if os(macOS) || os(iOS) || os(tvOS)
    if #available(macOS 11.0, iOS 14.0, tvOS 14.0, *) {
        return WADOSLogger()
    }
#endif
    
    return WADPrintLogger()
}()


internal let verboseLog: Bool = true
internal func wadLog(_ message: Any?) {
    guard verboseLog else {
        return
    }
    
    guard let message else {
        //print("nil")
        logger.log("nil")
        return
    }
    
    //print(String(describing: message))
    logger.log("\(String(describing: message))")
}
