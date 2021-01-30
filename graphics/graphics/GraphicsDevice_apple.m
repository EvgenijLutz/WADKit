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

static void* _metal_createTexture2dWithData(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, TEXTURE_USAGE usage, const void* data)
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

static void _metal_releaseTexture2d(GRAPHICS_DEVICE* device, void* textureId)
{
	id<MTLTexture> texture = (__bridge_transfer id<MTLTexture>)textureId;
	texture = nil;
}


// MARK: - Mesh interface

static void* _metal_createMesh(GRAPHICS_DEVICE* device, GRAPHICS_VERTEX* vertices, unsigned int numVertices, TEXTURE2D* texture)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	id<MTLDevice> metalDevice = graphicsDevice.metalDevice;
	
	size_t bufferSize = sizeof(GRAPHICS_VERTEX) * numVertices;
	id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithLength:bufferSize options:MTLResourceStorageModePrivate];
	id<MTLBuffer> uploadBuffer = [metalDevice newBufferWithLength:bufferSize options:MTLResourceStorageModeShared];
	memcpy(uploadBuffer.contents, vertices, bufferSize);
	
	id<MTLCommandBuffer> commandBuffer = [graphicsDevice.uploadDataCommandQueue commandBuffer];
	id<MTLBlitCommandEncoder> blit = [commandBuffer blitCommandEncoder];
	[blit copyFromBuffer:uploadBuffer sourceOffset:0 toBuffer:vertexBuffer destinationOffset:0 size:bufferSize];
	[blit endEncoding];
	[commandBuffer commit];
	[commandBuffer waitUntilCompleted];
	
	id<MTLTexture> metalTexture = (__bridge id<MTLTexture>)texture->textureId;
	
	GraphicsMesh* graphicsMesh = [[GraphicsMesh alloc] initWithVertexBuffer:vertexBuffer texture:metalTexture];
	void* graphicsMeshId = (__bridge_retained void*)graphicsMesh;
	return graphicsMeshId;
}

static void _metal_releaseMesh(GRAPHICS_MESH* mesh)
{
	GraphicsMesh* graphicsMesh = (__bridge_transfer GraphicsMesh*)mesh->meshId;
	graphicsMesh = nil;
}


// MARK: - Mesh uniforms interface

static void* _metal_createMeshUniforms(GRAPHICS_DEVICE* device)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	id<MTLDevice> metalDevice = graphicsDevice.metalDevice;
	
	size_t bufferSize = sizeof(GRAPHICS_MESH_UNIFORMS_DATA);
	
	GraphicsBuffer* uniforms = [[GraphicsBuffer alloc] initWithDevice:metalDevice size:bufferSize];
	void* uniformsId = (__bridge_retained void*)uniforms;
	return uniformsId;
}

static void _metal_releaseMeshUniforms(GRAPHICS_MESH_UNIFORMS* uniforms)
{
	GraphicsBuffer* meshUniforms = (__bridge_transfer GraphicsBuffer*)uniforms->uniformsId;
	meshUniforms = nil;
}

static void _metal_meshUniformsSetData(GRAPHICS_MESH_UNIFORMS* uniforms, COMMAND_QUEUE* queue, GRAPHICS_MESH_UNIFORMS_DATA* data)
{
	GraphicsBuffer* meshUniforms = (__bridge GraphicsBuffer*)uniforms->uniformsId;
	id<MTLCommandQueue> commandQueue = (__bridge id<MTLCommandQueue>)queue->commandQueueId;
	
	id<MTLCommandBuffer> commandBuffer = [commandQueue commandBuffer];
	id<MTLBlitCommandEncoder> blit = [commandBuffer blitCommandEncoder];
	
	[meshUniforms switchBuffers];
	[meshUniforms uploadData:data withEncoder:blit];
	
	[blit endEncoding];
	[commandBuffer commit];
}


// MARK: - Command queue interface

static void* _metal_createCommandQueue(GRAPHICS_DEVICE* device)
{
	GraphicsDevice* graphicsDevice = (__bridge GraphicsDevice*)device->userInfo;
	id<MTLDevice> metalDevice = graphicsDevice.metalDevice;
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

static void _metal_renderCommandEncoderRenderMesh(RENDER_COMMAND_ENCODER* renderCommandEncoder, GRAPHICS_MESH* mesh, GRAPHICS_MESH_UNIFORMS* uniforms)
{
	GRAPHICS_DEVICE* graphicsDevice = renderCommandEncoder->commandBuffer->commandQueue->device;
	GraphicsDevice* device = (__bridge GraphicsDevice*)graphicsDevice->userInfo;
	
	id<MTLRenderCommandEncoder> encoder = (__bridge id<MTLRenderCommandEncoder>)renderCommandEncoder->renderCommandEncoderId;
	
	[encoder setDepthStencilState:device.depthStencilState];
	[encoder setRenderPipelineState:device.opaqueTriangleRenderState];
	[encoder setCullMode:MTLCullModeBack];
	
	GraphicsMesh* graphicsMesh = (__bridge GraphicsMesh*)mesh->meshId;
	GraphicsBuffer* graphicsMeshUniforms = (__bridge GraphicsBuffer*)uniforms->uniformsId;
	
	[encoder setFrontFacingWinding:MTLWindingCounterClockwise];
	[encoder setVertexBuffer:graphicsMesh.vertexBuffer offset:0 atIndex:0];
	[encoder setVertexBuffer:graphicsMeshUniforms.buffer offset:0 atIndex:1];
	[encoder setFragmentTexture:graphicsMesh.texture atIndex:0];
	[encoder setFragmentBuffer:graphicsMeshUniforms.buffer offset:0 atIndex:1];
	[encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:mesh->numVertices];
}


// MARK: - GraphicsDevice implementation

@implementation GraphicsDevice

- (instancetype)initWithSystem:(SYSTEM*)system metalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		_system = system;
		_metalDevice = metalDevice;
		
		NSString* bundlePath = [NSBundle.mainBundle pathForResource:@"WKGraphicsResources" ofType:@"bundle"];
		NSBundle* bundle = [[NSBundle alloc] initWithPath:bundlePath];
		//id<MTLLibrary> library = [metalDevice newDefaultLibrary];
		id<MTLLibrary> library = [metalDevice newDefaultLibraryWithBundle:bundle error:nil];
		assert(library);
		
		// MARK: GRAPHICS_DEVICE implementation
		_graphicsDevice = malloc(sizeof(GRAPHICS_DEVICE));
		stringInitializeWithUTF8String(&_graphicsDevice->name, metalDevice.name.UTF8String);
		
		_graphicsDevice->system = system;
		_graphicsDevice->accessSemaphore = systemCreateSemaphore(system, 1);

		magicArrayInitialize(&_graphicsDevice->textures, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(TEXTURE2D), 8);
		magicArrayInitialize(&_graphicsDevice->meshes, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(GRAPHICS_MESH), 8);
		magicArrayInitialize(&_graphicsDevice->meshUniforms, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(GRAPHICS_MESH_UNIFORMS), 8);
		magicArrayInitialize(&_graphicsDevice->commandQueues, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(COMMAND_QUEUE), 8);
		//magicArrayInitialize(&_graphicsDevice->commandBuffers, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(COMMAND_BUFFER), 8);
		magicArrayInitialize(&_graphicsDevice->commandBuffers, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(COMMAND_BUFFER), 1);
		magicArrayInitialize(&_graphicsDevice->renderCommandEncoders, MAGIC_ARRAY_ITEM_DISTRIBUTION_DONT_CARE, sizeof(RENDER_COMMAND_ENCODER), 8);
		
		_graphicsDevice->createTexture2dWithDataFunc = _metal_createTexture2dWithData;
		_graphicsDevice->releaseTexture2dFunc = _metal_releaseTexture2d;
		
		_graphicsDevice->createMeshFunc = _metal_createMesh;
		_graphicsDevice->releaseMeshFunc = _metal_releaseMesh;
		
		_graphicsDevice->createMeshUniformsFunc = _metal_createMeshUniforms;
		_graphicsDevice->meshUniformsReleaseFunc = _metal_releaseMeshUniforms;
		_graphicsDevice->meshUniformsSetDataFunc = _metal_meshUniformsSetData;
		
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
		
		// MARK: Upload command queue
		_uploadDataCommandQueue = [metalDevice newCommandQueue];
		
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
	
	assert(_graphicsDevice->meshUniforms.length == 0);
	magicArrayDeinitialize(&_graphicsDevice->meshUniforms);
	
	assert(_graphicsDevice->meshes.length == 0);
	magicArrayDeinitialize(&_graphicsDevice->meshes);
	
	assert(_graphicsDevice->textures.length == 0);
	magicArrayDeinitialize(&_graphicsDevice->textures);
	
	semaphoreRelease(_graphicsDevice->accessSemaphore);
	
	stringDeinitialize(&_graphicsDevice->name);
	
	debug_memset(_graphicsDevice, 0, sizeof(GRAPHICS_DEVICE));
	free(_graphicsDevice);
}

@end
