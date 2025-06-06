// swift-tools-version: 6.0
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

func getDepandencies() -> [Package.Dependency] {
#if true
    // Local dependencies for development
    [
        .package(path: "../../DataIO"),
    ]
#else
    // Remote dependencies for release
    [
        .package(url: "https://github.com/EvgenijLutz/DataIO.git", exact: .init(1, 0, 0)),
    ]
#endif
}

let package = Package(
    name: "WADKit",
    platforms: [
        .macOS(.v15),
        .iOS(.v18)
    ],
    products: [
        .library(
            name: "WADKit",
            targets: ["WADKit"]
        ),
    ],
    dependencies: getDepandencies(),
    targets: [
        .target(
            name: "WADKit",
            dependencies: [
                .product(name: "DataIO", package: "DataIO")
            ]
        ),
    ]
)
