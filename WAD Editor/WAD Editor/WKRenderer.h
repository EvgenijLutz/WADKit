//
//  WKRenderer.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;

NS_ASSUME_NONNULL_BEGIN

@interface WKRenderer : NSObject

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice;

// -(instancetype)initWithRendererHandle:(id<RendererHandle>)handle;

//- (void)drawSubMeshWithVertexBuffer:(id<MTLBuffer>)vertexBuffer indexBuffer:(id<MTLBuffer>)indexBuffer numIndices:(NSUInteger)numIndices texture:(id<MTLTexture>)texture;

@property (readonly) id<MTLCommandQueue> drawCommandQueue;

// This value is set by delegate
// TODO: replace in future with MTLRenderPassDescriptor to allow more flexible drawing system
@property (nonatomic) id<MTLRenderCommandEncoder> renderEncoder;

@end

NS_ASSUME_NONNULL_END
