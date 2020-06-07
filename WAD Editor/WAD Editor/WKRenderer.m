//
//  WKRenderer.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WKRenderer.h"
#import "Submesh.h"
#import "MeshReflection.h"

@implementation WKRenderer
{
	__weak id<MTLDevice> device;
	
	id<MTLDepthStencilState> depthStencilState;
	id<MTLRenderPipelineState> drawSubmeshPipelineState;
	id<MTLRenderPipelineState> drawColoredLinePipelineState;
	
	id<MTLBuffer> boxLineVertexBuffer;
	id<MTLBuffer> boxLineIndexBuffer;
	unsigned int numBoxLineIndices;
}

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		device = metalDevice;
		
		// TODO: is it unused? delete!
		_drawCommandQueue = [metalDevice newCommandQueue];
		
		
		//===== Initialize pipeline states =====
		
		// Depth/stencil state
		{
			MTLDepthStencilDescriptor* depthStateDesc = [[MTLDepthStencilDescriptor alloc] init];
			depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
			depthStateDesc.depthWriteEnabled = YES;
			depthStencilState = [device newDepthStencilStateWithDescriptor:depthStateDesc];
		}
		
		id<MTLLibrary> library = [device newDefaultLibrary];
		assert(library);
		
		MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
		pipelineDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatBGRA8Unorm;
		pipelineDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
		pipelineDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
		
		/*
		 RGB =   (Source.rgb * sourceRGBBlendFactor) + (Dest.rgb * destinationRGBBlendFactor)
		 Alpha = (Source.a * sourceAlphaBlendFactor) + (Dest.a * destinationAlphaBlendFactor)
		 */
		pipelineDescriptor.colorAttachments[0].blendingEnabled = YES;
		
		pipelineDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
		pipelineDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
		pipelineDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
		
		pipelineDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
		pipelineDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
		pipelineDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
		
		// Textured triangle pipeline state
		{
			pipelineDescriptor.label = @"Textured triangle pipeline";
			pipelineDescriptor.vertexFunction = [library newFunctionWithName:@"texturedTriangle_vs"];
			pipelineDescriptor.fragmentFunction = [library newFunctionWithName:@"texturedTriangle_fs"];
			
			NSError* error;
			drawSubmeshPipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
			assert(drawSubmeshPipelineState);
		}
		
		// Colored line pipeline state
		{
			// 1. Create pipeline state
			
			pipelineDescriptor.label = @"Colored line pipeline";
			pipelineDescriptor.vertexFunction = [library newFunctionWithName:@"coloredLine_vs"];
			pipelineDescriptor.fragmentFunction = [library newFunctionWithName:@"coloredLine_fs"];
			
			NSError* error;
			drawColoredLinePipelineState = [device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:&error];
			assert(drawColoredLinePipelineState);
			
			
			// 2. Create shaders data
			
			// 2.1 Prepare blit stuff
			id<MTLCommandQueue> blitCommandQueue = [device newCommandQueue];
			id<MTLCommandBuffer> blitCommandBuffer = [blitCommandQueue commandBuffer];
			id<MTLBlitCommandEncoder> blitCommandEncoder = [blitCommandBuffer blitCommandEncoder];
			
			// 2.2 Create vertex buffer
			
			NSUInteger bufferLength = sizeof(WE_LINE_VERTEX) * 8;
			id<MTLBuffer> tempBuffer = [device newBufferWithLength:bufferLength options:MTLResourceStorageModeShared];
			WE_LINE_VERTEX* vertices = (WE_LINE_VERTEX*)tempBuffer.contents;
			
			// Front quad
			vertices[0].position = simd_make_float3(-0.5f, -0.5f,  0.5f);
			vertices[1].position = simd_make_float3( 0.5f, -0.5f,  0.5f);
			vertices[2].position = simd_make_float3( 0.5f,  0.5f,  0.5f);
			vertices[3].position = simd_make_float3(-0.5f,  0.5f,  0.5f);
			
			// Back quad
			vertices[4].position = simd_make_float3(-0.5f, -0.5f, -0.5f);
			vertices[5].position = simd_make_float3( 0.5f, -0.5f, -0.5f);
			vertices[6].position = simd_make_float3( 0.5f,  0.5f, -0.5f);
			vertices[7].position = simd_make_float3(-0.5f,  0.5f, -0.5f);
			
			boxLineVertexBuffer = [device newBufferWithLength:bufferLength options:MTLResourceStorageModePrivate];
			[blitCommandEncoder copyFromBuffer:tempBuffer sourceOffset:0 toBuffer:boxLineVertexBuffer destinationOffset:0 size:bufferLength];
			
			// 2.3 Create index buffer
			numBoxLineIndices = 24;
			bufferLength = sizeof(WE_LINE_INDEX) * numBoxLineIndices;
			tempBuffer = [device newBufferWithLength:bufferLength options:MTLResourceStorageModeShared];
			WE_LINE_INDEX* indices = (WE_LINE_INDEX*)tempBuffer.contents;
			
			// Front quad
			indices[0].index = 0;  indices[1].index = 1;
			indices[2].index = 1;  indices[3].index = 2;
			indices[4].index = 2;  indices[5].index = 3;
			indices[6].index = 3;  indices[7].index = 0;
			
			// Back quad
			indices[8].index  = 4; indices[9].index  = 5;
			indices[10].index = 5; indices[11].index = 6;
			indices[12].index = 6; indices[13].index = 7;
			indices[14].index = 7; indices[15].index = 4;
			
			// Connections of quads
			indices[16].index = 0; indices[17].index = 4;
			indices[18].index = 1; indices[19].index = 5;
			indices[20].index = 2; indices[21].index = 6;
			indices[22].index = 3; indices[23].index = 7;
			
			boxLineIndexBuffer = [device newBufferWithLength:bufferLength options:MTLResourceStorageModePrivate];
			[blitCommandEncoder copyFromBuffer:tempBuffer sourceOffset:0 toBuffer:boxLineIndexBuffer destinationOffset:0 size:bufferLength];
			
			// 2.4 Upload data to gpu
			[blitCommandEncoder endEncoding];
			[blitCommandBuffer commit];
			[blitCommandBuffer waitUntilCompleted];
		}
	}
	return self;
}

- (void)drawMesh:(MeshReflection*)meshReflection withUniforms:(OBJECT_UNIFORMS*)uniforms
{
	[_renderEncoder setDepthStencilState:depthStencilState];
	[_renderEncoder setRenderPipelineState:drawSubmeshPipelineState];
	[_renderEncoder setCullMode:MTLCullModeBack];
	[_renderEncoder setFrontFacingWinding:MTLWindingCounterClockwise];
	
	[_renderEncoder setVertexBuffer:meshReflection.vertexBuffer offset:0 atIndex:0];
	[_renderEncoder setVertexBytes:uniforms length:sizeof(OBJECT_UNIFORMS) atIndex:2];
	
	const unsigned int numSubmeshes = [meshReflection numSubmeshes];
	for (unsigned int submeshIndex = 0; submeshIndex < numSubmeshes; submeshIndex++)
	{
		Submesh* submesh = [meshReflection submeshAtIndex:submeshIndex];
		[_renderEncoder setVertexBuffer:submesh.indexBuffer offset:0 atIndex:1];
		[_renderEncoder setFragmentTexture:submesh.texture atIndex:0];
		
		[_renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:submesh.numIndices];
	}
}

- (void)drawCubeWithUniforms:(WE_LINE_UNIFORMS*)uniforms
{
	[_renderEncoder setDepthStencilState:depthStencilState];
	[_renderEncoder setRenderPipelineState:drawColoredLinePipelineState];
	[_renderEncoder setVertexBuffer:boxLineVertexBuffer offset:0 atIndex:0];
	[_renderEncoder setVertexBuffer:boxLineIndexBuffer offset:0 atIndex:1];
	[_renderEncoder setVertexBytes:uniforms length:sizeof(WE_LINE_UNIFORMS) atIndex:2];
	[_renderEncoder setFragmentBytes:uniforms length:sizeof(WE_LINE_UNIFORMS) atIndex:0];
	[_renderEncoder drawPrimitives:MTLPrimitiveTypeLine vertexStart:0 vertexCount:numBoxLineIndices];
}

@end
