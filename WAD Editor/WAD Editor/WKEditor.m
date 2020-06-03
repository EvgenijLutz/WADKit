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

@implementation WKEditor
{
	id<MTLDevice> device;
	ResourceReflectionStorage* storage;
	WAD* wad;
}

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice
{
	self = [super init];
	if (self)
	{
		device = metalDevice;
		storage = [[ResourceReflectionStorage alloc] initWithMetalDevice:device];
		wad = NULL;
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
}

- (void)drawWithRenderer:(WKRenderer*)renderer
{
	//
}

@end
