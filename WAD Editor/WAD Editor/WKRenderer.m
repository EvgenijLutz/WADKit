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
}

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		device = metalDevice;
		
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
		
		if (submeshIndex == 2)
		{
			submeshIndex = 2;
		}
	}
}

@end
