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
	float cameraDistance;
	simd_float2 rotation;
	unsigned int selectedObjectIndex;
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
		cameraDistance = 0.5f;
		rotation = simd_make_float2(0.0f, 0.0f);
		selectedObjectIndex = 0;
		
		_delegate = nil;
	}
	return self;
}

- (void)selectNextObject
{
	if (selectedObjectIndex + 1 >= [storage numMeshes])
	{
		selectedObjectIndex = 0;
		return;
	}
	
	selectedObjectIndex++;
}

- (void)selectPreviousObject
{
	if (selectedObjectIndex == 0)
	{
		selectedObjectIndex = [storage numMeshes] - 1;
		return;
	}
	
	selectedObjectIndex--;
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
	selectedObjectIndex = 290;	// palm
	selectedObjectIndex = 47;	// angry face
	
	[_delegate editor:self wadLoaded:wad];
}

- (WAD*)wad
{
	return wad;
}

#pragma mark - Editor actions

- (void)selectMeshAtIndex:(unsigned int)meshIndex
{
	assert(meshIndex < wadGetNumMeshes(wad));
	selectedObjectIndex = meshIndex;
}

#pragma mark - Protocol implementation

- (void)scaleView:(float)camDistance
{
	cameraDistance += camDistance;
	if (cameraDistance < 0.02f)
	{
		cameraDistance = 0.02f;
	}
}

- (void)rotateView:(simd_float2)rotationDelta
{
	rotation += rotationDelta;
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
	
	simd_float4x4 model = matrix_identity_float4x4;
	
	simd_float4x4 view = matrix_identity_float4x4;
	view = simd_mul(matrix4fRotation(rotation.y, simd_make_float3(0.0f, 1.0f, 0.0f)), view);
	view = simd_mul(matrix4fRotation(rotation.x, simd_make_float3(1.0f, 0.0f, 0.0f)), view);
	view = simd_mul(matrix4fTranslation(0.0f, 0.0f, -cameraDistance), view);
	
	const float fovyradians = 65.0 * (M_PI / 180.0f);
	simd_float4x4 projection = matrix4fPerspectiveRightHand(fovyradians, size.x / size.y, 0.01f, 1000.0f);
	
	OBJECT_UNIFORMS uniforms;
	uniforms.modelViewProjection = simd_mul(view, model);
	uniforms.modelViewProjection = simd_mul(projection, uniforms.modelViewProjection);
	
	MeshReflection* meshReflection = [storage meshAtIndex:selectedObjectIndex];
	[renderer drawMesh:meshReflection withUniforms:&uniforms];
}

@end
