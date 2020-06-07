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
#include "editor_selected_entity_type.h"

@implementation WKEditor
{
	id<MTLDevice> device;
	ResourceReflectionStorage* storage;
	WAD* wad;
	
	simd_float2 size;
	BOOL loaded;
	float cameraDistance;
	simd_float2 rotation;
	
	SELECTED_ENTITY_TYPE selectedEntityType;
	unsigned int selectedEntityIndex;
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
		
		selectedEntityType = SELECTED_ENTITY_TYPE_NONE;
		selectedEntityIndex = 0;
		
		_delegate = nil;
	}
	return self;
}

- (void)selectNextObject
{
	/*if (selectedEntityIndex + 1 >= [storage numMeshes])
	{
		selectedEntityIndex = 0;
		return;
	}
	
	selectedEntityIndex++;*/
}

- (void)selectPreviousObject
{
	/*if (selectedEntityIndex == 0)
	{
		selectedEntityIndex = [storage numMeshes] - 1;
		return;
	}
	
	selectedEntityIndex--;*/
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
	selectedEntityIndex = 290;	// palm
	selectedEntityIndex = 47;	// angry face
	
	[_delegate editor:self wadLoaded:wad];
}

- (WAD*)wad
{
	return wad;
}

#pragma mark - Editor actions

- (void)deselectEntity
{
	selectedEntityType = SELECTED_ENTITY_TYPE_NONE;
	selectedEntityIndex = 0;
}

- (void)selectMeshAtIndex:(unsigned int)meshIndex
{
	assert(meshIndex < wadGetNumMeshes(wad));
	
	selectedEntityType = SELECTED_ENTITY_TYPE_MESH;
	selectedEntityIndex = meshIndex;
}

- (void)selectMovableAtIndex:(unsigned int)movableIndex
{
	assert(movableIndex < wadGetNumMovables(wad));
	
	selectedEntityType = SELECTED_ENTITY_TYPE_MOVABLE;
	selectedEntityIndex = movableIndex;
}

- (void)selectStaticAtIndex:(unsigned int)staticIndex
{
	assert(staticIndex < wadGetNumStatics(wad));
	
	selectedEntityType = SELECTED_ENTITY_TYPE_STATIC;
	selectedEntityIndex = staticIndex;
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
	
	if (selectedEntityType == SELECTED_ENTITY_TYPE_MESH ||
		selectedEntityType == SELECTED_ENTITY_TYPE_MOVABLE ||
		selectedEntityType == SELECTED_ENTITY_TYPE_STATIC)
	{
		simd_float4x4 model = matrix_identity_float4x4;
		
		simd_float4x4 view = matrix_identity_float4x4;
		view = simd_mul(matrix4fRotation(rotation.y, simd_make_float3(0.0f, 1.0f, 0.0f)), view);
		view = simd_mul(matrix4fRotation(rotation.x, simd_make_float3(1.0f, 0.0f, 0.0f)), view);
		view = simd_mul(matrix4fTranslation(0.0f, 0.0f, -cameraDistance), view);
		
		const float fovyradians = 45.0 * (M_PI / 180.0f);
		simd_float4x4 projection = matrix4fPerspectiveRightHand(fovyradians, size.x / size.y, 0.01f, 1000.0f);
		
		OBJECT_UNIFORMS uniforms;
		uniforms.modelViewProjection = simd_mul(view, model);
		uniforms.modelViewProjection = simd_mul(projection, uniforms.modelViewProjection);
		
		if (selectedEntityType == SELECTED_ENTITY_TYPE_MESH)
		{
			unsigned int meshIndex = selectedEntityIndex;
			MeshReflection* meshReflection = [storage meshAtIndex:meshIndex];
			[renderer drawMesh:meshReflection withUniforms:&uniforms];
		}
		else if (selectedEntityType == SELECTED_ENTITY_TYPE_MOVABLE)
		{
			MOVABLE* movable = wadGetMovableByIndex(wad, selectedEntityIndex);
			unsigned int numMovableMeshes = movableGetNumMeshes(movable);
			if (numMovableMeshes > 0)
			{
				unsigned int meshIndex = movableGetMeshIndex(movable, 0);
				if (meshIndex == 0)
				{
					WE_LINE_UNIFORMS lineUniforms;
					lineUniforms.scale = 0.015f;
					lineUniforms.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
					lineUniforms.modelViewProjection = uniforms.modelViewProjection;
					
					[renderer drawCubeWithUniforms:&lineUniforms];
				}
				else
				{
					MeshReflection* meshReflection = [storage meshAtIndex:meshIndex];
					[renderer drawMesh:meshReflection withUniforms:&uniforms];
				}
				
				// FIXME: Don't use fixed number for matrix stack length
				unsigned int stackIndex = 1;
				simd_float4x4 stackMatrices[32];	// sch💩ße
				stackMatrices[0] = model;
				stackMatrices[1] = model;
				
				// FIXME: Don't use fixed number for parent stack length
				unsigned int parentIndex = 0;
				unsigned int parentMatrixIndices[32];
				parentMatrixIndices[parentIndex] = stackIndex;
				
				unsigned int skeletonIndex = movableGetSkeletonIndex(movable);
				SKELETON* skeleton = wadGetSkeleton(wad, skeletonIndex);
				
				for (unsigned int movableMeshIndex = 1; movableMeshIndex < numMovableMeshes; movableMeshIndex++)
				{
					unsigned int meshIndex = movableGetMeshIndex(movable, movableMeshIndex);
					MeshReflection* meshReflection = [storage meshAtIndex:meshIndex];
					
					JOINT* joint = skeletonGetJoint(skeleton, movableMeshIndex - 1);
					
					float jointX = ((float)jointGetOffsetX(joint)) / 1024.0f;
					float jointY = ((float)jointGetOffsetY(joint)) / 1024.0f;
					float jointZ = ((float)jointGetOffsetZ(joint)) / 1024.0f;
					simd_float4x4 translation = matrix4fTranslation(jointX, -jointY, -jointZ);
					
					JOINT_LOCATION_TYPE jointLinkType = jointGetLocationType(joint);
					if (jointLinkType == JOINT_LOCATION_TYPE_LINK_TO_PREVIOUS_MESH)
					{
						// do nothing lol
					}
					else if (jointLinkType == JOINT_LOCATION_TYPE_PULL_PARENT_FROM_STACK_AND_CONNECT)
					{
						if (parentIndex)
						{
							parentIndex--;
							stackIndex = parentMatrixIndices[parentIndex];
						}
						else
						{
							// This never should happen!
							// But sometimes it does.
							// Place breakpoint here if you want to catch the error.
							parentIndex = 0;
						}
					}
					else if (jointLinkType == JOINT_LOCATION_TYPE_PUSH_PARENT_TO_STACK_AND_CONNECT)
					{
						stackIndex++;
						stackMatrices[stackIndex] = stackMatrices[stackIndex - 1];
						
						parentIndex++;
						parentMatrixIndices[parentIndex] = stackIndex;
					}
					else if (jointLinkType == JOINT_LOCATION_TYPE_LINK_TO_PARENT_IN_STACK)
					{
						stackIndex = parentMatrixIndices[parentIndex];
						stackMatrices[stackIndex] = stackMatrices[stackIndex - 1];
					}
					
					// why not
					assert(stackIndex < 32);
					assert(parentIndex < 32);

					stackMatrices[stackIndex] = matrix_multiply(stackMatrices[stackIndex], translation);
					uniforms.modelViewProjection = simd_mul(view, stackMatrices[stackIndex]);
					uniforms.modelViewProjection = simd_mul(projection, uniforms.modelViewProjection);
					
					if (meshIndex == 0)
					{
						WE_LINE_UNIFORMS lineUniforms;
						lineUniforms.scale = 0.015f;
						lineUniforms.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
						lineUniforms.modelViewProjection = uniforms.modelViewProjection;
						
						[renderer drawCubeWithUniforms:&lineUniforms];
					}
					else
					{
						[renderer drawMesh:meshReflection withUniforms:&uniforms];
					}
				}
			}
		}
		else if (selectedEntityType == SELECTED_ENTITY_TYPE_STATIC)
		{
			STATIC* staticObject = wadGetStaticByIndex(wad, selectedEntityIndex);
			unsigned int meshIndex = staticGetMeshIndex(staticObject);
			MeshReflection* meshReflection = [storage meshAtIndex:meshIndex];
			[renderer drawMesh:meshReflection withUniforms:&uniforms];
		}
	}
}

@end
