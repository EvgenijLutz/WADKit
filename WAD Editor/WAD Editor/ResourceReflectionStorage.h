//
//  ResourceReflectionStorage.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;

#import "wad_kit_link.h"

@class MeshReflection;

NS_ASSUME_NONNULL_BEGIN

@interface ResourceReflectionStorage : NSObject

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice;

- (void)clear;

- (void)createTextureWithData:(unsigned char*)data atIndex:(unsigned int)textureIndex blitCommandEncoder:(id<MTLBlitCommandEncoder>)blitCommandEncoder;
- (id<MTLTexture>)textureAtIndex:(unsigned int)textureIndex;

- (unsigned int)numMeshes;
- (void)createMeshWithMeshData:(MESH*)mesh atIndex:(unsigned int)meshIndex wad:(WAD*)wad blitCommandEncoder:(id<MTLBlitCommandEncoder>)blitCommandEncoder;
- (MeshReflection*)meshAtIndex:(unsigned int)meshIndex;

@end

NS_ASSUME_NONNULL_END
