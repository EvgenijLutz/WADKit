//
//  GraphicsBuffer_apple.m
//  graphics macOS
//
//  Created by Евгений Лютц on 23.01.21.
//

#import "GraphicsBuffer_apple.h"

@implementation GraphicsBufferItem
@end


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
	
	NSMutableArray<GraphicsBufferItem*>* _items;
	NSUInteger _firstAvailableItem;
	//NSUInteger _lastEngagedItem;
}

- (instancetype)initWithDevice:(id<MTLDevice>)device itemSize:(size_t)size capacityIncrement:(size_t)capacityIncrement numBuffers:(unsigned int)numBuffers type:(GraphicsBufferType)type
{
	self = [super init];
	if (self)
	{
		_device = device;
		_itemSize = size;
		_capacityIncrement = capacityIncrement;
		_type = type;
		
		_numBuffers = numBuffers;
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
		
		_items = [[NSMutableArray alloc] initWithCapacity:capacityIncrement];
		for (unsigned int i = 0; i < capacityIncrement; i++)
		{
			GraphicsBufferItem* item = [[GraphicsBufferItem alloc] init];
			item.buffer = self;
			item.index = i;
			item.engaged = NO;
			
			[_items addObject:item];
		}
		_firstAvailableItem = 0;
		//_lastEngagedItem = 0;
	}
	return self;
}


- (GraphicsBufferItem*)addItem
{
	if (_firstAvailableItem == _items.count)
	{
		NSUInteger oldCapacity = _items.count;
		NSUInteger oldSize = oldCapacity * _itemSize;
		NSUInteger newCapacity = oldCapacity + _capacityIncrement;
		NSUInteger newSize = newCapacity * _itemSize;
		for (unsigned int i = 0; i < _numBuffers; i++)
		{
			BufferComponent* component = _components[i];
			
			id<MTLBuffer> uploadBuffer = [_device newBufferWithLength:newSize options:MTLResourceStorageModeShared];
			id<MTLBuffer> buffer = [_device newBufferWithLength:newSize options:MTLResourceStorageModePrivate];
			
			memcpy(uploadBuffer.contents, component.uploadBuffer.contents, oldSize);
			
			component.uploadBuffer = uploadBuffer;
			component.buffer = buffer;
		}
		
		for (unsigned int i = 0; i < _capacityIncrement; i++)
		{
			GraphicsBufferItem* item = [[GraphicsBufferItem alloc] init];
			item.buffer = self;
			item.index = oldCapacity + i;
			item.engaged = NO;
			[_items addObject:item];
		}
	}
	
	GraphicsBufferItem* selectedItem = _items[_firstAvailableItem];
	selectedItem.engaged = YES;
	
	_firstAvailableItem++;
	for (NSUInteger i = _firstAvailableItem; i < _items.count; i++)
	{
		GraphicsBufferItem* currentItem = _items[i];
		if (!currentItem.engaged)
		{
			break;
		}
		
		_firstAvailableItem++;
	}
	
	return selectedItem;
}

- (void)removeItem:(GraphicsBufferItem*)item
{
	item.engaged = NO;
	if (item.index < _firstAvailableItem)
	{
		_firstAvailableItem = item.index;
	}
}

- (void)setData:(const void*)data forItem:(GraphicsBufferItem*)item
{
	BufferComponent* component = _components[_currentBuffer];
	
	char* contents = component.uploadBuffer.contents;
	void* dataStart = contents + item.index * _itemSize;
	memcpy(dataStart, data, _itemSize);
}

- (void)scheduleUploadDataWithEncoder:(id<MTLBlitCommandEncoder>)blitEncoder
{
	// Switch to next buffer
	unsigned int lastBufferIndex = _currentBuffer;
	_currentBuffer = (_currentBuffer + 1) % _numBuffers;
	
	// Crutch
	if (lastBufferIndex != _currentBuffer)
	{
		id<MTLBuffer> lastBuffer = _components[lastBufferIndex].uploadBuffer;
		id<MTLBuffer> currentBuffer = _components[_currentBuffer].uploadBuffer;
		memcpy(currentBuffer.contents, lastBuffer.contents, lastBuffer.length);
	}
	
	BufferComponent* src = _components[lastBufferIndex];
	BufferComponent* dst = _components[_currentBuffer];
	NSUInteger bufferSize = _items.count * _itemSize;
	[blitEncoder copyFromBuffer:src.uploadBuffer sourceOffset:0 toBuffer:dst.buffer destinationOffset:0 size:bufferSize];
}

- (void)setLabel:(NSString*)label
{
	for (NSUInteger i = 0; i < _components.count; i++)
	{
		BufferComponent* component = _components[i];		
		if (label)
		{
			component.buffer.label = [NSString stringWithFormat:@"%@ (buffer #%lu/%lu)", label, i, _components.count];
			component.uploadBuffer.label = [NSString stringWithFormat:@"%@ (upload buffer #%lu/%lu)", label, i, _components.count];
		}
		else
		{
			component.buffer.label = nil;
			component.uploadBuffer.label = nil;
		}
	}
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
