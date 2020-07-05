//
//  ResourceStorage.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;

#import "wad_editor_lib_link.h"

@class MeshReflection;

NS_ASSUME_NONNULL_BEGIN

@interface ResourceStorage : NSObject

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice;

- (void)clear;

- (void)beginUploadData;
- (void)finishUploadData;

- (void)createTextureWithData:(TEXTURE_PAGE*)texturePage named:(const char*)name;
- (id<MTLTexture>)textureAtTexturePage:(TEXTURE_PAGE*)texturePage;
- (id<MTLTexture>)textureAtIndex:(unsigned int)textureIndex;

- (unsigned int)numMeshes;
- (void)createMeshWithMeshData:(MESH*)mesh;
- (MeshReflection*)meshAtMesh:(MESH*)mesh;
- (MeshReflection*)meshAtIndex:(unsigned int)meshIndex /*__attribute__((objc_direct))*/;

@property (nonatomic, readonly) id<MTLDevice> device;
@property (nonatomic, readonly) RESOURCE_STORAGE* resourceStorage;

@end

NS_ASSUME_NONNULL_END
