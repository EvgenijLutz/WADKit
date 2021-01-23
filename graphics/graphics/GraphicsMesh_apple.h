//
//  GraphicsMesh_apple.h
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

@import Metal;
#import "graphics_apple.h"

NS_ASSUME_NONNULL_BEGIN

@interface GraphicsMesh : NSObject

- (instancetype)initWithVertexBuffer:(id<MTLBuffer>)vertexBuffer texture:(id<MTLTexture>)texture;

@property (nonatomic, readonly) id<MTLBuffer> vertexBuffer;
@property (nonatomic, readonly) id<MTLTexture> texture;

@end

NS_ASSUME_NONNULL_END
