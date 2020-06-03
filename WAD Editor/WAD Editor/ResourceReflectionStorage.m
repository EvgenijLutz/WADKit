//
//  ResourceReflectionStorage.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "ResourceReflectionStorage.h"
#import "TextureReflection.h"
#import "Submesh.h"
#import "MeshReflection.h"
#import "SubmeshGenerator.h"
#include "RendererTypes.h"

@implementation ResourceReflectionStorage
{
	id<MTLDevice> device;
	NSMutableArray<TextureReflection*>* textureReflections;
	NSMutableArray<MeshReflection*>* meshReflections;
}

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		device = metalDevice;
		textureReflections = [[NSMutableArray alloc] init];
		meshReflections = [[NSMutableArray alloc] init];
	}
	return self;
}

- (void)clear
{
	for (TextureReflection* textureReflection in textureReflections)
	{
		textureReflection.texture = nil;
		textureReflection.index = 0;
	}
	
	[meshReflections removeAllObjects];
}

- (void)createTextureWithData:(unsigned char*)data atIndex:(unsigned int)textureIndex blitCommandEncoder:(id<MTLBlitCommandEncoder>)blitCommandEncoder
{
	const unsigned int textureSize = 256;
	const unsigned int numComponents = 4;
	const unsigned int bytesPerRow = textureSize * numComponents;
	
	unsigned char textureData[textureSize * numComponents * textureSize];
	
	unsigned char* dataPointer = data;
	unsigned char* textureDataPointer = textureData;
	for (unsigned int j = 0; j < textureSize; j++)
	{
		for (unsigned int i = 0; i < textureSize; i++)
		{
			*(textureDataPointer + 0) = *(dataPointer + 2);
			*(textureDataPointer + 1) = *(dataPointer + 1);
			*(textureDataPointer + 2) = *(dataPointer + 0);
			*(textureDataPointer + 3) = 255;
			
			dataPointer += 3;
			textureDataPointer += 4;
		}
	}
	
	MTLTextureDescriptor* textureDescriptor = [[MTLTextureDescriptor alloc] init];
	textureDescriptor.height = textureSize;
	textureDescriptor.width = textureSize;
	textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
	//textureDescriptor.storageMode = MTLStorageModePrivate;
	id<MTLTexture> texture = [device newTextureWithDescriptor:textureDescriptor];
	MTLRegion textureRegion = MTLRegionMake2D(0, 0, textureSize, textureSize);
	[texture replaceRegion:textureRegion mipmapLevel:0 slice:0 withBytes:textureData bytesPerRow:bytesPerRow bytesPerImage:textureSize * textureSize * numComponents];
	texture.label = [NSString stringWithFormat:@"Texture page #%03d", textureIndex];
	
	while (textureIndex >= textureReflections.count)
	{
		TextureReflection* reflection = [[TextureReflection alloc] init];
		[textureReflections addObject:reflection];
	}
	
	TextureReflection* reflection = textureReflections[textureIndex];
	reflection.texture = texture;
	reflection.index = textureIndex;
}

- (id<MTLTexture>)textureAtIndex:(unsigned int)textureIndex
{
	assert(textureIndex < textureReflections.count);
	
	TextureReflection* textureReflection = textureReflections[textureIndex];
	assert(textureReflection.index == textureIndex);
	return textureReflection.texture;
}

- (unsigned int)numMeshes
{
	return (unsigned int)(meshReflections.count);
}

- (void)createMeshWithMeshData:(MESH*)mesh atIndex:(unsigned int)meshIndex wad:(WAD*)wad blitCommandEncoder:(id<MTLBlitCommandEncoder>)blitCommandEncoder
{
	BOOL shaded = meshUsesNormals(mesh) == 0;
	
	// Create vertex buffer
	const unsigned int numVertices = meshGetNumVertices(mesh);
	const unsigned int meshVerticesSize = sizeof(WE_VERTEX) * numVertices;
	id<MTLBuffer> copyBuffer = [device newBufferWithLength:meshVerticesSize options:MTLResourceStorageModeShared];
	WE_VERTEX* meshVertices = (WE_VERTEX*)(copyBuffer.contents);
	for (unsigned int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		VERTEX vertex = meshGetVertex(mesh, vertexIndex);
		
		/*const float vx = ((float)(vertex.vx)) / 1024.0f;
		const float vy = ((float)(vertex.vy)) / 1024.0f;
		const float vz = ((float)(vertex.vz)) / 1024.0f;*/
		const float vx = ((float)(vertex.vx)) / 1024.0f;
		const float vy = -((float)(vertex.vy)) / 1024.0f;
		const float vz = -((float)(vertex.vz)) / 1024.0f;
		
		const float nx = ((float)(vertex.vx)) / 1024.0f;
		const float ny = ((float)(vertex.vy)) / 1024.0f;
		const float nz = ((float)(vertex.vz)) / 1024.0f;
		
		const float shade = ((float)(vertex.shade)) / 1024.0f;
		
		WE_VERTEX* meshVertex = &(meshVertices[vertexIndex]);
		meshVertex->position = simd_make_float3(vx, vy, vz);
		meshVertex->normal = simd_make_float3(nx, ny, nz);
		meshVertex->shade = shade;
	}
	
	// Upload vertex buffer to private storage
	id<MTLBuffer> vertexBuffer = [device newBufferWithLength:meshVerticesSize options:MTLResourceStorageModePrivate];
	[blitCommandEncoder copyFromBuffer:copyBuffer sourceOffset:0 toBuffer:vertexBuffer destinationOffset:0 size:meshVerticesSize];
	
	// Create submeshses
	SubmeshGenerator* submeshGenerator = [[SubmeshGenerator alloc] initWithStorage:self wad:wad];
	const unsigned int numPolygons = meshGetNumPolygons(mesh);
	for (unsigned int polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
	{
		POLYGON polygon = meshGetPolygon(mesh, polygonIndex);
		[submeshGenerator addPolygon:&polygon];
	}
	NSArray<Submesh*>* submeshes = [submeshGenerator generateSubmeshesWithBlitEncoder:blitCommandEncoder];
	
	MeshReflection* meshReflection = [[MeshReflection alloc] initWithVertexBuffer:vertexBuffer submeshes:submeshes shaded:shaded];
	
	[meshReflections addObject:meshReflection];
}

- (MeshReflection*)meshAtIndex:(unsigned int)meshIndex
{
	return meshReflections[meshIndex];
}

@end
