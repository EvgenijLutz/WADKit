//
//  GraphicsDevice_apple.m
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#import "GraphicsDevice_apple.h"
#include "private_interface.h"

// MARK: - Private interface

// MARK: - Texture interface

static void* _metal_createTexture2dWithData(GR_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, TEXTURE_USAGE usage, const void* data)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	id<MTLDevice> metalDevice = graphicsDevice.metalDevice;
	
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

static void _metal_releaseTexture2d(GR_DEVICE* device, void* textureId)
{
	id<MTLTexture> texture = (__bridge_transfer id<MTLTexture>)textureId;
	texture = nil;
}


// MARK: - Buffer interface

static void* _metal_createVertexBuffer(GR_DEVICE* device, COMMAND_QUEUE* queue, GR_T_VERTEX* vertices, unsigned int numVertices)
{
	size_t bufferSize = sizeof(GR_T_VERTEX) * numVertices;
	
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	id<MTLCommandQueue> commandQueue = (__bridge id<MTLCommandQueue>)queue->commandQueueId;
	
	GraphicsBufferItem* item = [graphicsDevice createVertexBufferWithSize:bufferSize];
	
	id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
	id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];
	[item.buffer setData:vertices forItem:item];
	[item.buffer scheduleUploadDataWithEncoder:blitEncoder];
	[blitEncoder endEncoding];
	[commandBuffer commit];
	
	void* itemId = (__bridge void*)item;
	return itemId;
}

static void* _metal_createMeshUniformsBuffer(GR_DEVICE* device)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	GraphicsBufferItem* item = [graphicsDevice createMeshUniformBuffer];
	void* itemId = (__bridge void*)item;
	return itemId;
}

static void* _metal_createViewportUniformsBuffer(GR_DEVICE* device)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	GraphicsBufferItem* item = [graphicsDevice createViewportUniformBuffer];
	void* itemId = (__bridge void*)item;
	return itemId;
}

static void _metal_setBufferData(GR_BUFFER* buffer, const void* data)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)buffer->device->userInfo;
	GraphicsBufferItem* item = (__bridge GraphicsBufferItem*)buffer->bufferId;
	[graphicsDevice bufferItem:item setData:data];
}

static void _metal_releaseBuffer(GR_BUFFER* buffer)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)buffer->device->userInfo;
	GraphicsBufferItem* item = (__bridge GraphicsBufferItem*)buffer->bufferId;
	[graphicsDevice releaseBufferItem:item];
}

static void _metal_setBufferLabel(GR_BUFFER* buffer, const char* label)
{
	GraphicsBufferItem* item = (__bridge GraphicsBufferItem*)buffer->bufferId;
	GraphicsBuffer* graphicsBuffer = item.buffer;
	
	NSString* labelString = nil;
	if (label)
	{
		labelString = [NSString stringWithUTF8String:label];
	}
	[graphicsBuffer setLabel:labelString];
}


static void _metal_scheduleUploadingBuffers(GR_DEVICE* device, COMMAND_BUFFER* commandBuffer)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	[graphicsDevice scheduleUploadBuffersWithCommandBuffer:metalCommandBuffer];
}


// MARK: - Command queue interface

static void* _metal_createCommandQueue(GR_DEVICE* device)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	id<MTLDevice> metalDevice = graphicsDevice.metalDevice;
	id<MTLCommandQueue> commandQueue = [metalDevice newCommandQueue];
	void* commandQueueId = (__bridge_retained void*)commandQueue;
	return commandQueueId;
}

static void _metal_releaseCommandQueue(GR_DEVICE* device, COMMAND_QUEUE* commandQueue)
{
	id<MTLCommandQueue> metalCommandQueue = (__bridge_transfer id<MTLCommandQueue>)commandQueue->commandQueueId;
	metalCommandQueue = nil;
}


// MARK: - Command buffer interface

static void* _metal_commandQueueCreateCommandBuffer(COMMAND_QUEUE* commandQueue)
{
	id<MTLCommandQueue> metalCommandQueue = (__bridge id<MTLCommandQueue>)commandQueue->commandQueueId;
	assert(metalCommandQueue);
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


// MARK: - Metal render command encoder

static void* _metal_commandBufferStartRenderCommandEncoder(COMMAND_BUFFER* commandBuffer, GRAPHICS_VIEW* graphicsView)
{
	GraphicsView* view = (__bridge GraphicsView*)graphicsView->userInfo;
	
	id<MTLCommandBuffer> metalCommandBuffer = (__bridge id<MTLCommandBuffer>)commandBuffer->commandBufferId;
	id<MTLRenderCommandEncoder> renderCommandEncoder = [metalCommandBuffer renderCommandEncoderWithDescriptor:view.currentRenderPassDescriptor];
	void* renderCommandEncoderId = (__bridge_retained void*)renderCommandEncoder;
	return renderCommandEncoderId;
}

static void _metal_renderCommandEncoderEndEncoding(RENDER_COMMAND_ENCODER* renderCommandEncoder)
{
	id<MTLRenderCommandEncoder> encoder = (__bridge_transfer id<MTLRenderCommandEncoder>)renderCommandEncoder->renderCommandEncoderId;
	[encoder endEncoding];
	
	encoder = nil;
}

static void _metal_renderCommandEncoderRenderMesh(RENDER_COMMAND_ENCODER* encoder, GR_BUFFER* vertexBuffer, unsigned int numVertices, TEXTURE2D* texture, GR_BUFFER* meshUniforms, GR_BUFFER* viewportUniforms)
{
	GR_DEVICE* device = encoder->commandBuffer->commandQueue->device;
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	id<MTLRenderCommandEncoder> renderEncoder = (__bridge id<MTLRenderCommandEncoder>)encoder->renderCommandEncoderId;
	
	GraphicsBufferItem* vertexBufferItem = (__bridge GraphicsBufferItem*)vertexBuffer->bufferId;
	assert(vertexBufferItem.buffer.type == GraphicsBufferTypeVertexBuffer);
	id<MTLBuffer> metalVertexBuffer = vertexBufferItem.buffer.buffer;
	
	id<MTLTexture> metalTexture = (__bridge id<MTLTexture>)texture->textureId;
	
	GraphicsBufferItem* meshUniformsBufferItem = (__bridge GraphicsBufferItem*)meshUniforms->bufferId;
	assert(meshUniformsBufferItem.buffer.type == GraphicsBufferTypeMeshUniforms);
	unsigned int meshUniformsBufferIndex = (unsigned int)meshUniformsBufferItem.index;
	id<MTLBuffer> meshUniformsBuffer = meshUniformsBufferItem.buffer.buffer;
	
	GraphicsBufferItem* viewportUniformBufferItem = (__bridge GraphicsBufferItem*)viewportUniforms->bufferId;
	assert(viewportUniformBufferItem.buffer.type == GraphicsBufferTypeViewportUniforms);
	id<MTLBuffer> viewportUniformBuffer = viewportUniformBufferItem.buffer.buffer;
	
	[renderEncoder setDepthStencilState:graphicsDevice.depthStencilState];
	[renderEncoder setRenderPipelineState:graphicsDevice.opaqueTriangleRenderState];
	[renderEncoder setCullMode:MTLCullModeBack];
	
	[renderEncoder setFrontFacingWinding:MTLWindingCounterClockwise];
	[renderEncoder setVertexBuffer:metalVertexBuffer offset:0 atIndex:0];
	[renderEncoder setVertexBytes:&meshUniformsBufferIndex length:sizeof(unsigned int) atIndex:1];
	[renderEncoder setVertexBuffer:meshUniformsBuffer offset:0 atIndex:2];
	[renderEncoder setVertexBuffer:viewportUniformBuffer offset:0 atIndex:3];
	[renderEncoder setFragmentTexture:metalTexture atIndex:0];
	[renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:numVertices];
}


// MARK: - GraphicsDevice implementation

@implementation GraphicsDevice
{
	/// Allocates GR_T_VERTEX and GR_TNW2_VERTEX buffers.
	NSMutableArray<GraphicsBuffer*>* _texturedMeshBuffers;
	
	/// Allocates GR_VIEWPORT_UNIFORMS_DATA buffers
	NSMutableArray<GraphicsBuffer*>* _viewportUniformBuffers;
	
	/// Allocates GR_MESH_UNIFORM_DATA
	GraphicsBuffer* _meshUniformsAllocator;
}

- (instancetype)initWithSystem:(SYSTEM*)system metalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		_system = system;
		_metalDevice = metalDevice;
		
		_texturedMeshBuffers = [[NSMutableArray alloc] init];
		_viewportUniformBuffers = [[NSMutableArray alloc] init];
		_meshUniformsAllocator = [[GraphicsBuffer alloc] initWithDevice:_metalDevice itemSize:sizeof(GR_MESH_UNIFORM_DATA) capacityIncrement:32 numBuffers:3 type:GraphicsBufferTypeMeshUniforms];
		
		NSString* bundlePath = [NSBundle.mainBundle pathForResource:@"WKGraphicsResources" ofType:@"bundle"];
		assert(bundlePath);
		NSBundle* bundle = [[NSBundle alloc] initWithPath:bundlePath];
		assert(bundle);
		id<MTLLibrary> library = [metalDevice newDefaultLibraryWithBundle:bundle error:nil];
		assert(library);
		
		// MARK: GR_DEVICE implementation
		_graphicsDevice = malloc(sizeof(GR_DEVICE));
		stringInitializeWithUTF8String(&_graphicsDevice->name, metalDevice.name.UTF8String);
		
		_graphicsDevice->system = system;
		_graphicsDevice->accessSemaphore = systemCreateSemaphore(system, 1);

		magicArrayInitialize(&_graphicsDevice->textures, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(TEXTURE2D), 8);
		magicArrayInitialize(&_graphicsDevice->buffers, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(GR_BUFFER), 64);
		magicArrayInitialize(&_graphicsDevice->commandQueues, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(COMMAND_QUEUE), 8);
		magicArrayInitialize(&_graphicsDevice->commandBuffers, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(COMMAND_BUFFER), 8);
		magicArrayInitialize(&_graphicsDevice->renderCommandEncoders, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(RENDER_COMMAND_ENCODER), 8);
		
		_graphicsDevice->createTexture2dWithDataFunc = _metal_createTexture2dWithData;
		_graphicsDevice->releaseTexture2dFunc = _metal_releaseTexture2d;
		
		_graphicsDevice->createVertexBufferFunc = _metal_createVertexBuffer;
		_graphicsDevice->createMeshUniformsBufferFunc = _metal_createMeshUniformsBuffer;
		_graphicsDevice->createViewportUniformsBufferFunc = _metal_createViewportUniformsBuffer;
		_graphicsDevice->setBufferDataFunc = _metal_setBufferData;
		_graphicsDevice->releaseBufferFunc = _metal_releaseBuffer;
		_graphicsDevice->setBufferLabelFunc = _metal_setBufferLabel;
		
		_graphicsDevice->scheduleUploadBuffersFunc = _metal_scheduleUploadingBuffers;
		
		_graphicsDevice->createCommandQueueFunc = _metal_createCommandQueue;
		_graphicsDevice->releaseCommandQueueFunc = _metal_releaseCommandQueue;

		_graphicsDevice->commandQueueCreateCommandBufferFunc = _metal_commandQueueCreateCommandBuffer;
		_graphicsDevice->commandQueueReleaseCommandBufferFunc = _metal_commandQueueReleaseCommandBuffer;

		_graphicsDevice->commandBufferAddCompletionFunc = _metal_commandBufferAddCompletion;
		_graphicsDevice->commandBufferPresentDrawableFunc = _metal_commandBufferPresentDrawable;
		_graphicsDevice->commandBufferCommitFunc = _metal_commandBufferCommit;
		_graphicsDevice->commandBufferWaitUntilCompletedFunc = _metal_commandBufferWaitUntilCompleted;

		_graphicsDevice->commandBufferStartRenderCommandEncoderFunc = _metal_commandBufferStartRenderCommandEncoder;
		_graphicsDevice->renderCommandEncoderEndEncodingFunc = _metal_renderCommandEncoderEndEncoding;
		_graphicsDevice->renderCommandEncoderRenderTexturedMeshFunc = _metal_renderCommandEncoderRenderMesh;
		
		_graphicsDevice->userInfo = (__bridge void*)self;
		
		// MARK: Default parameters (for GraphicsView)
		_defaultRenderClearColor = MTLClearColorMake(0.1, 0.1, 0.1, 1.0);
		_defaultRenderColorPixelFormat = MTLPixelFormatBGRA8Unorm;
		_defaultDepthStencilPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
		
		// MARK: Depth/stencil state
		MTLDepthStencilDescriptor* depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
		depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
		depthStateDesc.depthWriteEnabled = YES;
		_depthStencilState = [metalDevice newDepthStencilStateWithDescriptor:depthStateDesc];
		assert(_depthStencilState);
		
		// MARK: Textured triangle render pipeline state
		MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
		pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
		pipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
		pipelineDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
		
		// RGB =   (Source.rgb * sourceRGBBlendFactor) + (Dest.rgb * destinationRGBBlendFactor)
		// Alpha = (Source.a * sourceAlphaBlendFactor) + (Dest.a * destinationAlphaBlendFactor)

		pipelineDescriptor.colorAttachments[0].blendingEnabled = YES;
		
		pipelineDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
		pipelineDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
		pipelineDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
		
		pipelineDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
		pipelineDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
		pipelineDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
		
		pipelineDescriptor.label = @"Opaque textured triangle pipeline";
		pipelineDescriptor.vertexFunction = [library newFunctionWithName:@"opaqueTexturedTriangle_vs"];
		pipelineDescriptor.fragmentFunction = [library newFunctionWithName:@"opaqueTexturedTriangle_fs"];
		assert(pipelineDescriptor.vertexFunction);
		assert(pipelineDescriptor.fragmentFunction);
		
		NSError* error = nil;
		_opaqueTriangleRenderState = [metalDevice newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
		assert(_opaqueTriangleRenderState);
	}
	return self;
}

- (void)dealloc
{
	assert(_graphicsDevice->renderCommandEncoders.length == 0);
	magicArrayDeinitialize(&_graphicsDevice->renderCommandEncoders);
	
	assert(_graphicsDevice->commandBuffers.length == 0);
	magicArrayDeinitialize(&_graphicsDevice->commandBuffers);
	
	assert(_graphicsDevice->commandQueues.length == 0);
	magicArrayDeinitialize(&_graphicsDevice->commandQueues);
	
	assert(_graphicsDevice->buffers.length == 0);
	magicArrayDeinitialize(&_graphicsDevice->buffers);
	
	assert(_graphicsDevice->textures.length == 0);
	magicArrayDeinitialize(&_graphicsDevice->textures);
	
	semaphoreRelease(_graphicsDevice->accessSemaphore);
	
	stringDeinitialize(&_graphicsDevice->name);
	
	debug_memset(_graphicsDevice, 0, sizeof(GR_DEVICE));
	free(_graphicsDevice);
}


- (GraphicsBufferItem*)createVertexBufferWithSize:(size_t)size
{
	GraphicsBuffer* graphicsBuffer = [[GraphicsBuffer alloc] initWithDevice:_metalDevice itemSize:size capacityIncrement:1 numBuffers:1 type:GraphicsBufferTypeVertexBuffer];
	[_texturedMeshBuffers addObject:graphicsBuffer];
	
	return [graphicsBuffer addItem];
}

- (GraphicsBufferItem*)createMeshUniformBuffer
{
	return [_meshUniformsAllocator addItem];
}

- (GraphicsBufferItem*)createViewportUniformBuffer
{
	GraphicsBuffer* graphicsBuffer = [[GraphicsBuffer alloc] initWithDevice:_metalDevice itemSize:sizeof(GR_VIEWPORT_UNIFORMS_DATA) capacityIncrement:1 numBuffers:3 type:GraphicsBufferTypeViewportUniforms];
	[_viewportUniformBuffers addObject:graphicsBuffer];
	
	return [graphicsBuffer addItem];
}

- (void)bufferItem:(GraphicsBufferItem*)item setData:(const void*)data
{
	[item.buffer setData:data forItem:item];
}

- (void)releaseBufferItem:(GraphicsBufferItem*)item
{
	GraphicsBuffer* buffer = item.buffer;
	
	if (buffer.type == GraphicsBufferTypeVertexBuffer)
	{
		[_texturedMeshBuffers removeObject:buffer];
		return;
	}
	else if (buffer.type == GraphicsBufferTypeViewportUniforms)
	{
		[_viewportUniformBuffers removeObject:buffer];
		return;
	}
	
	[buffer removeItem:item];
}

- (void)scheduleUploadBuffersWithCommandBuffer:(id<MTLCommandBuffer>)commandBuffer
{
	id<MTLBlitCommandEncoder> blitEncoder = [commandBuffer blitCommandEncoder];
	
	for (GraphicsBuffer* buffer in _viewportUniformBuffers)
	{
		[buffer scheduleUploadDataWithEncoder:blitEncoder];
	}
	
	[_meshUniformsAllocator scheduleUploadDataWithEncoder:blitEncoder];
	
	[blitEncoder endEncoding];
}

@end
