//
//  GraphicsBuffer_apple.h
//  graphics macOS
//
//  Created by Евгений Лютц on 23.01.21.
//

@import Metal;
#include "graphics_apple.h"

NS_ASSUME_NONNULL_BEGIN

@interface GraphicsBufferItem : NSObject
@property (nonatomic, weak) GraphicsBuffer* buffer;
@property (nonatomic) NSUInteger index;
@property (nonatomic) BOOL engaged;
@end

typedef enum GraphicsBufferType
{
	GraphicsBufferTypeVertexBuffer = 0,
	GraphicsBufferTypeMeshUniforms,
	GraphicsBufferTypeViewportUniforms,
}
GraphicsBufferType;

@interface GraphicsBuffer : NSObject

- (instancetype)initWithDevice:(id<MTLDevice>)device itemSize:(size_t)size capacityIncrement:(size_t)capacityIncrement numBuffers:(unsigned int)numBuffers type:(GraphicsBufferType)type;

@property (nonatomic, readonly) id<MTLDevice> device;
@property (nonatomic, readonly) size_t itemSize;
@property (nonatomic, readonly) size_t capacityIncrement;
@property (nonatomic) GraphicsBufferType type;

- (GraphicsBufferItem*)addItem;
- (void)removeItem:(GraphicsBufferItem*)item;
- (void)setData:(const void*)data forItem:(GraphicsBufferItem*)item;
- (void)scheduleUploadDataWithEncoder:(id<MTLBlitCommandEncoder>)blitEncoder;

- (void)setLabel:(NSString*)label;

- (id<MTLBuffer>)uploadBuffer;
- (id<MTLBuffer>)buffer;

@end

NS_ASSUME_NONNULL_END
