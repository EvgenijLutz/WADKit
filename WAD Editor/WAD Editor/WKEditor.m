//
//  WKEditor.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "WKEditor.h"
#import "ResourceReflectionStorage.h"
#include "wad_kit_link.h"
#include "math_utils.h"
#include "RendererTypes.h"
#include "WKRenderer.h"

@implementation WKEditor
{
	id<MTLDevice> device;
	ResourceReflectionStorage* storage;
	WAD* wad;
	
	simd_float2 size;
	BOOL loaded;
	float rotation;
	float objectCounter;
}

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		device = metalDevice;
		storage = [[ResourceReflectionStorage alloc] initWithMetalDevice:device];
		wad = NULL;
		
		size = simd_make_float2(1.0f, 1.0f);
		loaded = NO;
		rotation = 0.0f;
		objectCounter = 0.0f;
	}
	return self;
}

- (void)loadWadByPath:(NSString*)wadPath
{
	// 1. Load wad data
	
	if (wad)
	{
		wadRelease(wad);
	}
	loaded = NO;
	
	[storage clear];
	
	NSData* wadData = [NSData dataWithContentsOfFile:wadPath];
	if (!wadData)
	{
		NSLog(@"Failed to read wad file");
		return;
	}
	
	EXECUTE_RESULT executeResult;
	wad = wadLoadFromWadData(wadData.bytes, wadData.length, &executeResult);
	if (!executeResult.succeeded)
	{
		NSLog(@"Failed to read wad file with message: %s", executeResult.message);
		return;
	}
	
	// 2. Fill resource storage
	id<MTLCommandQueue> blitCommandQueue = [device newCommandQueue];
	id<MTLCommandBuffer> blitCommandBuffer = [blitCommandQueue commandBuffer];
	id<MTLBlitCommandEncoder> blitCommandEncoder = [blitCommandBuffer blitCommandEncoder];
	
	const unsigned int numTexturePages = wadGetNumTexturePages(wad);
	for (unsigned int texturePageIndex = 0; texturePageIndex < numTexturePages; texturePageIndex++)
	{
		unsigned char* data = wadGetTexturePageData(wad, texturePageIndex);
		[storage createTextureWithData:data atIndex:texturePageIndex blitCommandEncoder:blitCommandEncoder];
	}
	
	const unsigned int numMeshes = wadGetNumMeshes(wad);
	for (unsigned int meshIndex = 0; meshIndex < numMeshes; meshIndex++)
	{
		MESH* mesh = wadGetMesh(wad, meshIndex);
		[storage createMeshWithMeshData:mesh atIndex:meshIndex wad:wad blitCommandEncoder:blitCommandEncoder];
	}
	
	[blitCommandEncoder endEncoding];
	[blitCommandBuffer commit];
	[blitCommandBuffer waitUntilCompleted];
	
	// 3. Display something
	
	loaded = YES;
}

- (void)sizeChanged:(simd_float2)viewportSize
{
	size = viewportSize;
}

- (void)drawWithRenderer:(WKRenderer*)renderer
{
	if (!loaded)
	{
		return;
	}
	
	rotation += 0.005;
	while (rotation > (M_PI * 2))
	{
		rotation -= (M_PI * 2);
	}
	
	OBJECT_UNIFORMS uniforms;
	simd_float4x4 model = matrix4fRotation(rotation, simd_make_float3(0.0f, 1.0f, 0.0f));
	simd_float4x4 view = matrix4fTranslation(0.0f, -1.0f, -3.0f);
	
	const float fovyradians = 65.0 * (M_PI / 180.0f);
	simd_float4x4 projection = matrix4fPerspectiveRightHand(fovyradians, size.x / size.y, 0.01f, 1000.0f);
	
	uniforms.modelViewProjection = simd_mul(view, model);
	uniforms.modelViewProjection = simd_mul(projection, uniforms.modelViewProjection);
	
	objectCounter += 0.02;
	//unsigned int meshIndex = 141;
	unsigned int meshIndex = 141;//(unsigned int)objectCounter;
	if (meshIndex >= [storage numMeshes])
	{
		meshIndex = 0;
		objectCounter = 0.0f;
	}
	
	MeshReflection* meshReflection = [storage meshAtIndex:meshIndex];
	[renderer drawMesh:meshReflection withUniforms:&uniforms];
}

@end
