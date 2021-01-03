//
//  apple_graphics_device.m
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

@import Metal;

#include "private_interface.h"
#include "apple_graphics_device.h"

/*
 Toll-Free Bridged Types:
 https://developer.apple.com/library/archive/documentation/CoreFoundation/Conceptual/CFDesignConcepts/Articles/tollFreeBridgedTypes.html
*/

static id<MTLDevice> _getMetalDevice(void* userInfo)
{
	return (__bridge id<MTLDevice>)userInfo;
}

static void* _apple_createTexture2d(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, const void* data)
{
	id<MTLDevice> metalDevice = _getMetalDevice(device->userInfo);
	
	MTLTextureDescriptor* descriptor = [[MTLTextureDescriptor alloc] init];
	descriptor.width = width;
	descriptor.height = height;
	descriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
	
	char* modifiedData = NULL;
	if (numComponents < 4)
	{
		const char* sourceData = (const char*)data;
		
		modifiedData = malloc(sizeof(char) * width * height * 4);
		unsigned int p = 0;
		unsigned int pointer = 0;
		for (unsigned int i = 0; i < height; i++)
		{
			for (unsigned int j = 0; j < width; j++)
			{
				for (unsigned int k = 0; k < numComponents; k++)
				{
					unsigned int n = p + numComponents - 1 - k;
					modifiedData[pointer] = sourceData[n];
					pointer++;
				}
				p += numComponents;
				
				for (unsigned int k = numComponents; k < 4; k++)
				{
					modifiedData[pointer] = 0xFF;
					pointer++;
				}
			}
		}
		
		numComponents = 4;
	}
	
	id<MTLTexture> texture = [metalDevice newTextureWithDescriptor:descriptor];
	
	MTLRegion textureRegion = MTLRegionMake2D(0, 0, width, height);
	const void* bytes = modifiedData ? modifiedData : data;
	const NSUInteger rowBytes = width * numComponents;
	const NSUInteger imageBytes = width * height * numComponents;
	[texture replaceRegion:textureRegion mipmapLevel:0 slice:0 withBytes:bytes bytesPerRow:rowBytes bytesPerImage:imageBytes];
	
	if (modifiedData)
	{
		free(modifiedData);
	}
	
	void* textureId = (__bridge_retained void*)texture;
	return textureId;
}

static void _apple_releaseTexture2d(GRAPHICS_DEVICE* device, void* textureId)
{
	id<MTLTexture> texture = (__bridge_transfer id<MTLTexture>)textureId;
	texture = nil;
}

GRAPHICS_DEVICE* apple_createDefaultGraphicsDevice(void)
{
	id<MTLDevice> metalDevice = MTLCreateSystemDefaultDevice();
	assert(metalDevice);
	
	GRAPHICS_DEVICE* device = malloc(sizeof(GRAPHICS_DEVICE));
	stringInitialize(&device->name, metalDevice.name.UTF8String);
	
	device->createTexture2dFunc = _apple_createTexture2d;
	device->releaseTexture2dFunc = _apple_releaseTexture2d;
	
	device->createdByDefault = 1;
	device->userInfo = (__bridge_retained void*)metalDevice;
	
	return device;
}

void apple_releaseDefaultGraphicsDevice(GRAPHICS_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	
	id<MTLDevice> metalDevice = (__bridge_transfer id<MTLDevice>)graphicsDevice->userInfo;
	metalDevice = nil;
	
	stringDeinitialize(&graphicsDevice->name);
	
	debug_memset(graphicsDevice, 0, sizeof(GRAPHICS_DEVICE));
	free(graphicsDevice);
}
