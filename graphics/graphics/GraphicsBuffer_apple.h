//
//  GraphicsBuffer_apple.h
//  graphics macOS
//
//  Created by Евгений Лютц on 23.01.21.
//

@import Metal;

NS_ASSUME_NONNULL_BEGIN

@interface GraphicsBuffer : NSObject

- (instancetype)initWithDevice:(id<MTLDevice>)device size:(size_t)size;

@property (nonatomic, readonly) id<MTLDevice> device;
@property (nonatomic, readonly) size_t size;

- (void)switchBuffers;
- (void)uploadData:(const void*)data withEncoder:(id<MTLBlitCommandEncoder>)blitEncoder;

- (id<MTLBuffer>)uploadBuffer;
- (id<MTLBuffer>)buffer;

@end

NS_ASSUME_NONNULL_END
