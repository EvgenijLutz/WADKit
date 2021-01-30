//
//  GraphicsDevice_apple.h
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

@import Metal;
#include "graphics_apple.h"

NS_ASSUME_NONNULL_BEGIN

__attribute__((objc_direct_members))
@interface GraphicsDevice : NSObject

- (instancetype)initWithSystem:(SYSTEM*)system metalDevice:(id<MTLDevice>)metalDevice;

@property (nonatomic, readonly) SYSTEM* system;
@property (nonatomic, readonly) id<MTLDevice> metalDevice;
@property (nonatomic, readonly) GRAPHICS_DEVICE* graphicsDevice;

@property (nonatomic, readonly) MTLClearColor defaultRenderClearColor;
@property (nonatomic, readonly) MTLPixelFormat defaultRenderColorPixelFormat;
@property (nonatomic, readonly) MTLPixelFormat defaultDepthStencilPixelFormat;

@property (nonatomic, readonly) id<MTLCommandQueue> uploadDataCommandQueue;

@property (nonatomic, readonly) id<MTLDepthStencilState> depthStencilState;
@property (nonatomic, readonly) id<MTLRenderPipelineState> opaqueTriangleRenderState;

@end

NS_ASSUME_NONNULL_END
