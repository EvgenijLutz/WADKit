//
//  ResourceStorage.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "ResourceStorage.h"
#import "TextureReflection.h"
#import "Submesh.h"
#import "MeshReflection.h"
#import "SubmeshGenerator.h"
#include "RendererTypes.h"

// MARK: - Resource storage delegate functions

static ResourceStorage* _resourceStorageFromUserInfo(void* userInfo)
{
	return (__bridge ResourceStorage*)userInfo;
}

static void _resourceStorageDelegateBeginUploadData(void* userInfo)
{
	ResourceStorage* resourceStorage = _resourceStorageFromUserInfo(userInfo);
	[resourceStorage beginUploadData];
}

static void _resourceStorageDelegateFinishUploadData(void* userInfo)
{
	ResourceStorage* resourceStorage = _resourceStorageFromUserInfo(userInfo);
	[resourceStorage finishUploadData];
}

static void _resourceStorageDelegateCreateTexture(TEXTURE_PAGE* texturePage, const char* name, void* userInfo)
{
	ResourceStorage* resourceStorage = _resourceStorageFromUserInfo(userInfo);
	[resourceStorage createTextureWithData:texturePage named:name];
}

static void _resourceStorageDelegateCreateMesh(MESH* mesh, void* userInfo)
{
	ResourceStorage* resourceStorage = _resourceStorageFromUserInfo(userInfo);
	[resourceStorage createMeshWithMeshData:mesh];
}

// MARK: - Resource storage implementation

// FIXME: uncomment in Xcode 12
//__attribute__((objc_direct_members))
@implementation ResourceStorage
{
	id<MTLCommandQueue> blitCommandQueue;
	id<MTLCommandBuffer> blitCommandBuffer;
	id<MTLBlitCommandEncoder> blitCommandEncoder;
	
	NSMutableArray<TextureReflection*>* textureReflections;
	NSMutableArray<MeshReflection*>* meshReflections;
}

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		_resourceStorage = resourceStorageCreate(_resourceStorageDelegateBeginUploadData, _resourceStorageDelegateFinishUploadData, _resourceStorageDelegateCreateTexture, _resourceStorageDelegateCreateMesh, (__bridge void*)self);
		
		_device = metalDevice;
		textureReflections = [[NSMutableArray alloc] init];
		meshReflections = [[NSMutableArray alloc] init];
	}
	return self;
}

- (void)dealloc
{
	resourceStorageRelease(_resourceStorage);
}

- (void)clear
{
	[textureReflections removeAllObjects];
	[meshReflections removeAllObjects];
}

- (void)beginUploadData
{
	blitCommandQueue = [_device newCommandQueue];
	blitCommandBuffer = [blitCommandQueue commandBuffer];
	blitCommandEncoder = [blitCommandBuffer blitCommandEncoder];
}

- (void)finishUploadData
{
	[blitCommandEncoder endEncoding];
	[blitCommandBuffer commit];
	[blitCommandBuffer waitUntilCompleted];
}

- (void)createTextureWithData:(TEXTURE_PAGE*)texturePage named:(const char*)name
{
	const unsigned int textureSize = 256;
	const unsigned int numComponents = 4;
	const unsigned int bytesPerRow = textureSize * numComponents;
	
	unsigned char textureData[textureSize * numComponents * textureSize];
	
	unsigned char* data = texturePageGetData(texturePage);
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
	id<MTLTexture> texture = [_device newTextureWithDescriptor:textureDescriptor];
	MTLRegion textureRegion = MTLRegionMake2D(0, 0, textureSize, textureSize);
	[texture replaceRegion:textureRegion mipmapLevel:0 slice:0 withBytes:textureData bytesPerRow:bytesPerRow bytesPerImage:textureSize * textureSize * numComponents];
	//texture.label = [NSString stringWithFormat:@"Texture page #%03d", textureIndex];
	texture.label = [NSString stringWithUTF8String:name];

	TextureReflection* reflection = [[TextureReflection alloc] initWithTexturePage:texturePage texture:texture];
	[textureReflections addObject:reflection];
}

- (id<MTLTexture>)textureAtTexturePage:(TEXTURE_PAGE*)texturePage
{
	TextureReflection* result = nil;
	for (TextureReflection* textureReflection in textureReflections)
	{
		if (textureReflection.texturePage == texturePage)
		{
			result = textureReflection;
			break;
		}
	}
	assert(result);
	
	return result.texture;
}

- (id<MTLTexture>)textureAtIndex:(unsigned int)textureIndex
{
	assert(textureIndex < textureReflections.count);
	return textureReflections[textureIndex].texture;
}

- (unsigned int)numMeshes
{
	return (unsigned int)(meshReflections.count);
}

- (void)createMeshWithMeshData:(MESH*)mesh
{
	assert(blitCommandEncoder);
	
	WAD* wad = meshGetWad(mesh);
	
	BOOL shaded = meshUsesNormals(mesh) == 0;
	
	// Create vertex buffer
	const unsigned int numVertices = meshGetNumVertices(mesh);
	const unsigned int meshVerticesSize = sizeof(WE_VERTEX) * numVertices;
	id<MTLBuffer> copyBuffer = [_device newBufferWithLength:meshVerticesSize options:MTLResourceStorageModeShared];
	WE_VERTEX* meshVertices = (WE_VERTEX*)(copyBuffer.contents);
	for (unsigned int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		VERTEX vertex = meshGetVertex(mesh, vertexIndex);
		
		const float vx = ((float)(vertex.vx)) / 1024.0f;
		const float vy = -((float)(vertex.vy)) / 1024.0f;
		const float vz = -((float)(vertex.vz)) / 1024.0f;
		
		const float nx = ((float)(vertex.vx)) / 2048.0f;//1024.0f;
		const float ny = ((float)(vertex.vy)) / 2048.0f;//1024.0f;
		const float nz = ((float)(vertex.vz)) / 2048.0f;//1024.0f;
		
		const float shade = ((float)(vertex.shade)) / 1024.0f;
		
		WE_VERTEX* meshVertex = &(meshVertices[vertexIndex]);
		meshVertex->position = simd_make_float3(vx, vy, vz);
		meshVertex->normal = simd_make_float3(nx, ny, nz);
		meshVertex->shade = shade;
	}
	
	// Upload vertex buffer to private storage
	id<MTLBuffer> vertexBuffer = [_device newBufferWithLength:meshVerticesSize options:MTLResourceStorageModePrivate];
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
	
	MeshReflection* meshReflection = [[MeshReflection alloc] initWithMesh:mesh vertexBuffer:vertexBuffer submeshes:submeshes shaded:shaded];
	
	[meshReflections addObject:meshReflection];
}

- (MeshReflection*)meshAtMesh:(MESH*)mesh
{
	MeshReflection* result = nil;
	for (MeshReflection* meshReflection in meshReflections)
	{
		if (meshReflection.mesh == mesh)
		{
			result = meshReflection;
			break;
		}
	}
	assert(result);
	
	return result;
}

- (MeshReflection*)meshAtIndex:(unsigned int)meshIndex
{
	return meshReflections[meshIndex];
}

@end
