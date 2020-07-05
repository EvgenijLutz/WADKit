//
//  SubmeshGenerator.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "SubmeshGenerator.h"
#import "ResourceStorage.h"
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
	
	float su = (float)(textureSample->x) / 256.0f;
	float sv = (float)(textureSample->y) / 256.0f;
	float eu = su + (float)(textureSample->addW) / 256.0f;
	float ev = sv + (float)(textureSample->addH) / 256.0f;
	
	/*const float duv = 0.5f / 256.0f;
	su = (float)(textureSample->x) / 256.0f + duv;
	sv = (float)(textureSample->y) / 256.0f + duv;
	eu = su + (float)(textureSample->addW) / 256.0f - duv;
	ev = sv + (float)(textureSample->addH) / 256.0f - duv;*/
	
	const int flipX = textureSample->flipX;
	if (!flipX)
	{
		const float temp = su;
		su = eu;
		eu = temp;
	}
	
	const int flipY = textureSample->flipY;
	if (!flipY)
	{
		const float temp = sv;
		sv = ev;
		ev = temp;
	}

	if (polygon->flipped)
	{
		const float temp = su;
		su = eu;
		eu = temp;
	}
	
	simd_float2 uvA = simd_make_float2(su, sv);
	simd_float2 uvB = simd_make_float2(eu, sv);
	simd_float2 uvC = simd_make_float2(eu, ev);
	simd_float2 uvD = simd_make_float2(su, ev);
	
	simd_float2 uv1 = simd_make_float2(0.0f, 0.0f);
	simd_float2 uv2 = simd_make_float2(1.0f, 0.0f);
	simd_float2 uv3 = simd_make_float2(1.0f, 1.0f);
	simd_float2 uv4 = simd_make_float2(0.0f, 1.0f);
	
	if (polygon->isTriangle)
	{
		if (polygon->textureSampleShape == 0)
		{
			uv1 = uvA;
			uv2 = uvB;
			uv3 = uvD;
		}
		else if (polygon->textureSampleShape == 2)
		{
			uv1 = uvB;
			uv2 = uvC;
			uv3 = uvA;
		}
		else if (polygon->textureSampleShape == 4)
		{
			uv1 = uvC;
			uv2 = uvD;
			uv3 = uvB;
		}
		else if (polygon->textureSampleShape == 6)
		{
			uv1 = uvD;
			uv2 = uvA;
			uv3 = uvC;
		}
	}
	else
	{
		uv1 = uvA;
		uv2 = uvB;
		uv3 = uvC;
		uv4 = uvD;
	}
	
	/*
	// omg debug stuff
	if (polygon->textureInfo == 0xFFEE && !polygon->isTriangle &&
		polygon->v1 == 16 && polygon->v2 == 15 && polygon->v3 == 14 && polygon->v4 == 13 &&
		textureSample->x == 240 && textureSample->y == 0 && textureSample->page == 5)
	{
		indices[numIndices + 2].vertexIndex = polygon->v2;
	}*/
	
	indices[numIndices + 0].uv = uv1;
	indices[numIndices + 1].uv = uv3;
	indices[numIndices + 2].uv = uv2;
	
	if (!polygon->isTriangle)
	{
		indices[numIndices + 3].vertexIndex = polygon->v1;
		indices[numIndices + 4].vertexIndex = polygon->v4;
		indices[numIndices + 5].vertexIndex = polygon->v3;
		
		indices[numIndices + 3].uv = uv1;
		indices[numIndices + 4].uv = uv4;
		indices[numIndices + 5].uv = uv3;
		
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
	ResourceStorage* storage;
	WAD* wad;
	
	NSMutableArray<SubmeshData*>* submeshesData;
}

- (instancetype)initWithStorage:(ResourceStorage*)resourceStorage wad:(WAD*)_wad
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
	
	TEXTURE_PAGE* texturePage = wadGetTexturePage(wad, textureIndex);
	id<MTLTexture> texture = [storage textureAtTexturePage:texturePage];//[storage textureAtIndex:textureIndex];
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
