//
//  GraphicsBuffer_apple.m
//  graphics macOS
//
//  Created by Евгений Лютц on 23.01.21.
//

#import "GraphicsBuffer_apple.h"

@interface BufferComponent : NSObject

@property (nonatomic) id<MTLBuffer> uploadBuffer;
@property (nonatomic) id<MTLBuffer> buffer;

@end
@implementation BufferComponent @end



@implementation GraphicsBuffer
{
	unsigned int _numBuffers;
	unsigned int _currentBuffer;
	
	NSArray<BufferComponent*>* _components;
}

- (instancetype)initWithDevice:(id<MTLDevice>)device size:(size_t)size
{
	self = [super init];
	if (self)
	{
		_device = device;
		_size = size;
		
		_numBuffers = 3;
		_currentBuffer = 0;
		
		NSMutableArray<BufferComponent*>* tempComponents = [[NSMutableArray alloc] init];
		for (unsigned int i = 0; i < _numBuffers; i++)
		{
			id<MTLBuffer> uploadBuffer = [device newBufferWithLength:size options:MTLResourceStorageModeShared];
			id<MTLBuffer> buffer = [device newBufferWithLength:size options:MTLResourceStorageModePrivate];
			
			BufferComponent* component = [[BufferComponent alloc] init];
			component.uploadBuffer = uploadBuffer;
			component.buffer = buffer;
			
			[tempComponents addObject:component];
		}
		
		_components = tempComponents;
	}
	return self;
}


- (void)switchBuffers
{
	_currentBuffer = (_currentBuffer + 1) % _numBuffers;
}

- (void)uploadData:(const void*)data withEncoder:(id<MTLBlitCommandEncoder>)blitEncoder
{
	BufferComponent* component = _components[_currentBuffer];
	memcpy(component.uploadBuffer.contents, data, _size);
	
	[blitEncoder copyFromBuffer:component.uploadBuffer sourceOffset:0 toBuffer:component.buffer destinationOffset:0 size:_size];
}


- (id<MTLBuffer>)uploadBuffer
{
	return _components[_currentBuffer].uploadBuffer;
}

- (id<MTLBuffer>)buffer
{
	return _components[_currentBuffer].buffer;
}

@end
