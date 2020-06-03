//
//  SubmeshGenerator.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "SubmeshGenerator.h"
#import "ResourceReflectionStorage.h"
#import "Submesh.h"
#include "RendererTypes.h"

//===== Submesh data =====

@interface SubmeshData : NSObject

- (instancetype)initWithTextureIndex:(unsigned int)textureIndex texture:(id<MTLTexture>)texture;

- (void)addPolygon:(POLYGON*)polygon withTextureSample:(TEXTURE_SAMPLE*)textureSample;
- (Submesh*)createSubmeshWithBlitEncoder:(id<MTLBlitCommandEncoder>)blitCommandEncoder;

@property (readonly) unsigned int textureIndex;
@property (readonly) id<MTLTexture> texture;

@end

@implementation SubmeshData
{
	unsigned int numIndices;
	unsigned int indicesCapacity;
	WE_INDEX* indices;
}

- (instancetype)initWithTextureIndex:(unsigned int)textureIndex texture:(id<MTLTexture>)texture
{
	self = [super init];
	if (self)
	{
		numIndices = 0;
		indicesCapacity = 8;
		indices = malloc(sizeof(WE_INDEX) * indicesCapacity);
		
		_textureIndex = textureIndex;
		_texture = texture;
	}
	return self;
}

- (void)dealloc
{
	free(indices);
}

- (void)addPolygon:(POLYGON*)polygon withTextureSample:(TEXTURE_SAMPLE*)textureSample
{
	if (numIndices + 6 >= indicesCapacity)
	{
		indicesCapacity += 128;
		WE_INDEX* newIndices = realloc(indices, sizeof(WE_INDEX) * indicesCapacity);
		assert(newIndices);
		indices = newIndices;
	}
	
	indices[numIndices + 0].vertexIndex = polygon->v1;
	indices[numIndices + 1].vertexIndex = polygon->v3;
	indices[numIndices + 2].vertexIndex = polygon->v2;
	if (polygon->isTriangle)
	{
		indices[numIndices + 3].vertexIndex = polygon->v1;
		indices[numIndices + 4].vertexIndex = polygon->v4;
		indices[numIndices + 5].vertexIndex = polygon->v3;
		numIndices += 6;
	}
	else
	{
		numIndices += 3;
	}
}

- (Submesh*)createSubmeshWithBlitEncoder:(id<MTLBlitCommandEncoder>)blitCommandEncoder
{
	NSUInteger bufferSize = sizeof(WE_INDEX) * numIndices;
	id<MTLBuffer> copyBuffer = [blitCommandEncoder.device newBufferWithLength:bufferSize options:MTLResourceStorageModeShared];
	memcpy(copyBuffer.contents, indices, bufferSize);
	
	id<MTLBuffer> indexBuffer = [blitCommandEncoder.device newBufferWithLength:bufferSize options:MTLResourceStorageModePrivate];
	
	[blitCommandEncoder copyFromBuffer:copyBuffer sourceOffset:0 toBuffer:indexBuffer destinationOffset:0 size:bufferSize];
	
	Submesh* submesh = [[Submesh alloc] initWithIndexBuffer:indexBuffer numIndices:numIndices texture:_texture];
	return submesh;
}

@end

//===== Submesh generator implementation =====

@implementation SubmeshGenerator
{
	ResourceReflectionStorage* storage;
	WAD* wad;
	
	NSMutableArray<SubmeshData*>* submeshesData;
}

- (instancetype)initWithStorage:(ResourceReflectionStorage*)resourceStorage wad:(WAD*)_wad
{
	self = [super init];
	if (self)
	{
		storage = resourceStorage;
		wad = _wad;
		submeshesData = [[NSMutableArray alloc] init];
	}
	return self;
}

- (nonnull SubmeshData*)_getSubmeshDataForTextureIndex:(unsigned int)textureIndex
{
	for (SubmeshData* submeshData in submeshesData)
	{
		if (submeshData.textureIndex == textureIndex)
		{
			return submeshData;
		}
	}
	
	id<MTLTexture> texture = [storage textureAtIndex:textureIndex];
	SubmeshData* submeshData = [[SubmeshData alloc] initWithTextureIndex:textureIndex texture:texture];
	[submeshesData addObject:submeshData];
	
	return submeshData;
}

- (void)addPolygon:(POLYGON*)polygon
{
	TEXTURE_SAMPLE textureSample = wadGetTextureSample(wad, polygon->textureSampleIndex);
	SubmeshData* submeshData = [self _getSubmeshDataForTextureIndex:textureSample.page];
	[submeshData addPolygon:polygon withTextureSample:&textureSample];
}

- (NSArray<Submesh*>*)generateSubmeshesWithBlitEncoder:(id<MTLBlitCommandEncoder>)blitCommandEncoder
{
	NSMutableArray<Submesh*>* submeshes =  [[NSMutableArray alloc] init];
	for (SubmeshData* submeshData in submeshesData)
	{
		Submesh* submesh = [submeshData createSubmeshWithBlitEncoder:blitCommandEncoder];
		[submeshes addObject:submesh];
	}
	
	return submeshes;
}

@end
