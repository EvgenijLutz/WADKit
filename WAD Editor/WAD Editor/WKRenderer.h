//
//  WKRenderer.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;
#include "RendererTypes.h"
#include "wad_editor_lib_link.h"

@class ResourceStorage;
@class MeshReflection;

NS_ASSUME_NONNULL_BEGIN

@interface WKRenderer : NSObject

- (instancetype)initWithResourceStorage:(ResourceStorage*)storage;

- (void)drawMesh:(MESH*)mesh model:(matrix4f*)model view:(matrix4f*)view projection:(matrix4f*)projection;

- (void)drawMesh:(MeshReflection*)meshReflection withUniforms:(OBJECT_UNIFORMS*)uniforms;
- (void)drawCubeWithUniforms:(WE_LINE_UNIFORMS*)uniforms;

@property (readonly) RENDERER* renderer;
@property (readonly) id<MTLCommandQueue> drawCommandQueue;

// This value is set by delegate
// TODO: replace in future with MTLRenderPassDescriptor to allow more flexible drawing system
@property (nonatomic) id<MTLRenderCommandEncoder> renderEncoder;

@end

NS_ASSUME_NONNULL_END
