//
//  apple_graphics_device.m
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

@import Metal;

#include "wad_editor_lib_apple.h"
#include "private_interface.h"

/*
 Toll-Free Bridged Types:
 https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFDesignConcepts/Articles/tollFreeBridgedTypes.html
*/

static id<MTLDevice> _getMetalDevice(void* userInfo)
{
	id<MTLDevice> device = (__bridge id<MTLDevice>)userInfo;
	assert(device);
	return device;
}


// MARK: - Texture interface

static void* _metal_createTexture2d(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, TEXTURE_USAGE usage, const void* data)
{
	id<MTLDevice> metalDevice = _getMetalDevice(device->userInfo);
	
	MTLTextureDescriptor* descriptor = [[MTLTextureDescriptor alloc] init];
	descriptor.width = width;
	descriptor.height = height;
	descriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
	
	MTLTextureUsage textureUsage = MTLTextureUsageUnknown;
	if (usage & TEXTURE_USAGE_SHADER_READ) {
		textureUsage |= MTLTextureUsageShaderRead;
	}
	if (usage & TEXTURE_USAGE_SHADER_WRITE) {
		textureUsage |= MTLTextureUsageShaderWrite;
	}
	if (usage & TEXTURE_USAGE_RENDER_TARGET) {
		textureUsage |= MTLTextureUsageRenderTarget;
	}
	descriptor.usage = textureUsage;
	
	// Modify input data if needed
	char* modifiedData = NULL;
	if (numComponents < 4)
	{
		const char* sourceData = (const char*)data;
		
		modifiedData = malloc(sizeof(char) * width * height * 4);
		unsigned int p = 0;
		unsigned int pointer = 0;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				for (unsigned int k = 0; k < numComponents; k++)
				{
					// rgb -> bgra
					unsigned int n = p + numComponents - 1 - k;
					modifiedData[pointer] = sourceData[n];
					pointer++;
				}
				p += numComponents;
				
				// Fill other bits with 0xFF
				for (unsigned int k = numComponents; k < 4; k++)
				{
					modifiedData[pointer] = 0xFF;
					pointer++;
				}
			}
		}
		
		numComponents = 4;
	}
	
	id<MTLTexture> texture = [metalDevice newTextureWithDescriptor:descriptor];
	
	MTLRegion textureRegion = MTLRegionMake2D(0, 0, width, height);
	const void* bytes = modifiedData ? modifiedData : data;
	const NSUInteger rowBytes = width * numComponents;
	const NSUInteger imageBytes = width * height * numComponents;
	[texture replaceRegion:textureRegion mipmapLevel:0 slice:0 withBytes:bytes bytesPerRow:rowBytes bytesPerImage:imageBytes];
	
	if (modifiedData)
	{
		free(modifiedData);
	}
	
	void* textureId = (__bridge_retained void*)texture;
	return textureId;
}

static void _metal_releaseTexture2d(GRAPHICS_DEVICE* device, void* textureId)
{
	id<MTLTexture> texture = (__bridge_transfer id<MTLTexture>)textureId;
	texture = nil;
}


// MARK: - Buffer interface

static void* _metal_createBuffer(GRAPHICS_DEVICE* device, unsigned int length, GRAPHICS_BUFFER_OPTION option)
{
	id<MTLDevice> metalDevice = _getMetalDevice(device->userInfo);
	
	MTLResourceOptions resourceOptions = 0;
	if (option == GRAPHICS_BUFFER_OPTION_CPU_READ_WRITE) {
		resourceOptions = MTLResourceCPUCacheModeDefaultCache | MTLResourceStorageModeManaged;
	}
	else if (option == GRAPHICS_BUFFER_OPTION_GPU_ONLY) {
		resourceOptions = MTLResourceStorageModePrivate;
	}
	
	id<MTLBuffer> buffer = [metalDevice newBufferWithLength:length options:resourceOptions];
	void* bufferId = (__bridge_retained void*)buffer;
	return bufferId;
}

static void _metal_releaseBuffer(GRAPHICS_DEVICE* device, void* bufferId)
{
	id<MTLBuffer> buffer = (__bridge_transfer id<MTLBuffer>)bufferId;
	buffer = nil;
}

static void _metal_graphicsBufferSetLabel(GRAPHICS_BUFFER* graphicsBuffer, const char* text)
{
	id<MTLBuffer> buffer = (__bridge id<MTLBuffer>)graphicsBuffer->bufferId;
	buffer.label = [NSString stringWithUTF8String:text];
}

static void* _metal_graphicsBufferGetDataToCPUWrite(GRAPHICS_BUFFER* graphicsBuffer)
{
	id<MTLBuffer> buffer = (__bridge id<MTLBuffer>)graphicsBuffer->bufferId;
	return buffer.contents;
}


// MARK: - Command queue interface

static void* _metal_createCommandQueue(GRAPHICS_DEVICE* device)
{
	id<MTLDevice> metalDevice = _getMetalDevice(device->userInfo);
	id<MTLCommandQueue> commandQueue = [metalDevice newCommandQueue];
	void* commandQueueId = (__bridge_retained void*)commandQueue;
	return commandQueueId;
}

static void _metal_releaseCommandQueue(GRAPHICS_DEVICE* device, COMMAND_QUEUE* commandQueue)
{
	id<MTLCommandQueue> metalCommandQueue = (__bridge_transfer id<MTLCommandQueue>)commandQueue->commandQueueId;
	metalCommandQueue = nil;
}


// MARK: - Command buffer interface

static void* _metal_commandQueueCreateCommandBuffer(COMMAND_QUEUE* commandQueue)
{
	id<MTLCommandQueue> metalCommandQueue = (__bridge id<MTLCommandQueue>)commandQueue->commandQueueId;
	id<MTLCommandBuffer> metalCommandBuffer = [metalCommandQueue commandBuffer];
	void* commandBufferId = (__bridge_retained void*)metalCommandBuffer;
	return commandBufferId;
}

static void _metal_commandQueueReleaseCommandBuffer(COMMAND_QUEUE* commandQueue, COMMAND_BUFFER* commandBuffer)
{
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge_transfer id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	metalCommandBuffer = nil;
}

static void _metal_commandBufferAddCompletion(COMMAND_BUFFER* commandBuffer, COMMAND_BUFFER_COMPLETION_FUNC completionFunc, void* userInfo)
{
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	[metalCommandBuffer addCompletedHandler:^(id<MTLCommandBuffer> _Nonnull buf) {
		completionFunc(commandBuffer, userInfo);
	}];
}

static void _metal_commandBufferPresentDrawable(COMMAND_BUFFER* commandBuffer, GRAPHICS_DRAWABLE* graphicsDrawable)
{
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	id<MTLDrawable> metalDrawable = (__bridge id<MTLDrawable>)graphicsDrawable->drawableId;
	[metalCommandBuffer presentDrawable:metalDrawable];
}

static void _metal_commandBufferCommit(COMMAND_BUFFER* commandBuffer)
{
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	[metalCommandBuffer commit];
}

static void _metal_commandBufferWaitUntilCompleted(COMMAND_BUFFER* commandBuffer)
{
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	[metalCommandBuffer waitUntilCompleted];
}


// MARK: - Metal blit command encoder

static void* _metal_commandBufferStartBlitCommandEncoder(COMMAND_BUFFER* commandBuffer)
{
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	id<MTLBlitCommandEncoder> blitCommandEncoder = [metalCommandBuffer blitCommandEncoder];
	void* blitCommandEncoderId = (__bridge_retained void*)blitCommandEncoder;
	return blitCommandEncoderId;
}

static void _metal_blitCommandEncoderEndEncoding(BLIT_COMMAND_ENCODER* blitCommandEncoder)
{
	id<MTLBlitCommandEncoder> encoder = (__bridge_transfer id<MTLBlitCommandEncoder>)blitCommandEncoder->blitCommandEncoderId;
	[encoder endEncoding];
	
	encoder = nil;
}

static void _metal_blitCommandEncoderCopyFromBufferToBuffer(BLIT_COMMAND_ENCODER* blitCommandEncoder, GRAPHICS_BUFFER* sourceBuffer, unsigned long sourceOffset, GRAPHICS_BUFFER* destinationBuffer, unsigned long destinationOffset, unsigned long size)
{
	id<MTLBlitCommandEncoder> encoder = (__bridge id<MTLBlitCommandEncoder>)blitCommandEncoder->blitCommandEncoderId;
	id<MTLBuffer> source = (__bridge id<MTLBuffer>)sourceBuffer->bufferId;
	id<MTLBuffer> destination = (__bridge id<MTLBuffer>)destinationBuffer->bufferId;
	
	[encoder copyFromBuffer:source sourceOffset:sourceOffset toBuffer:destination destinationOffset:destinationOffset size:size];
}


// MARK: - Metal render command encoder

void _metal_commandBufferTestFunction___DO_NOT_CALL_THIS_FUNCTION___(COMMAND_BUFFER* commandBuffer)
{
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	
	MTLRenderPipelineDescriptor* renderPipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
	renderPipelineDescriptor.colorAttachments[0].blendingEnabled = YES;
	NSError* error = nil;
	id<MTLDevice> device = metalCommandBuffer.device;
	id<MTLRenderPipelineState> renderPipelineState = [device newRenderPipelineStateWithDescriptor:renderPipelineDescriptor error:&error];
	
	MTLRenderPassDescriptor* descriptor = [[MTLRenderPassDescriptor alloc] init];
	
	id<MTLRenderCommandEncoder> encoder = [metalCommandBuffer renderCommandEncoderWithDescriptor:descriptor];
	[encoder setRenderPipelineState:renderPipelineState];
}

/*static void* _metal_commandBufferStartRenderCommandEncoder(COMMAND_BUFFER* commandBuffer)
{
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	return NULL;
}*/


// MARK: - Metal device interface

GRAPHICS_DEVICE* graphicsDeviceCreateDefaultMetalDevice(void)
{
	id<MTLDevice> metalDevice = MTLCreateSystemDefaultDevice();
	assert(metalDevice);
	
	GRAPHICS_DEVICE* device = malloc(sizeof(GRAPHICS_DEVICE));
	stringInitialize(&device->name, metalDevice.name.UTF8String);
	device->createdByDefault = 1;
	
	arrayInitialize(&device->textures, sizeof(TEXTURE2D));
	arrayInitialize(&device->buffers, sizeof(GRAPHICS_BUFFER));
	arrayInitialize(&device->commandBuffers, sizeof(COMMAND_BUFFER));
	arrayInitialize(&device->blitCommandEncoders, sizeof(BLIT_COMMAND_ENCODER));
	
	device->createTexture2dFunc = _metal_createTexture2d;
	device->releaseTexture2dFunc = _metal_releaseTexture2d;
	
	device->createBufferFunc = _metal_createBuffer;
	device->graphicsBufferReleaseFunc = _metal_releaseBuffer;
	device->graphicsBufferSetLabelFunc = _metal_graphicsBufferSetLabel;
	device->graphicsBufferGetDataToCPUWriteFunc = _metal_graphicsBufferGetDataToCPUWrite;
	
	device->createCommandQueueFunc = _metal_createCommandQueue;
	device->releaseCommandQueueFunc = _metal_releaseCommandQueue;
	
	device->commandQueueCreateCommandBufferFunc = _metal_commandQueueCreateCommandBuffer;
	device->commandQueueReleaseCommandBufferFunc = _metal_commandQueueReleaseCommandBuffer;
	
	device->commandBufferAddCompletionFunc = _metal_commandBufferAddCompletion;
	device->commandBufferPresentDrawableFunc = _metal_commandBufferPresentDrawable;
	device->commandBufferCommitFunc = _metal_commandBufferCommit;
	device->commandBufferWaitUntilCompletedFunc = _metal_commandBufferWaitUntilCompleted;
	
	device->commandBufferStartBlitCommandEncoderFunc = _metal_commandBufferStartBlitCommandEncoder;
	device->blitCommandEncoderEndEncodingFunc = _metal_blitCommandEncoderEndEncoding;
	device->blitCommandEncoderCopyFromBufferToBuffer = _metal_blitCommandEncoderCopyFromBufferToBuffer;
	
	device->userInfo = (__bridge_retained void*)metalDevice;
	
	return device;
}

void graphicsDeviceReleaseDefaultMetalDevice(GRAPHICS_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	
	id<MTLDevice> metalDevice = (__bridge_transfer id<MTLDevice>)graphicsDevice->userInfo;
	metalDevice = nil;
	
	assert(graphicsDevice->blitCommandEncoders.length == 0);
	arrayDeinitialize(&graphicsDevice->blitCommandEncoders);
	
	assert(graphicsDevice->commandBuffers.length == 0);
	arrayDeinitialize(&graphicsDevice->commandBuffers);
	
	assert(graphicsDevice->buffers.length == 0);
	arrayDeinitialize(&graphicsDevice->buffers);
	
	assert(graphicsDevice->textures.length == 0);
	arrayDeinitialize(&graphicsDevice->textures);
	
	stringDeinitialize(&graphicsDevice->name);
	
	debug_memset(graphicsDevice, 0, sizeof(GRAPHICS_DEVICE));
	free(graphicsDevice);
}
