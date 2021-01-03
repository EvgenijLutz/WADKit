//
//  graphics_device_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

#if defined(__APPLE__) && __APPLE__
#include "apple_graphics_device.h"
#endif

GRAPHICS_DEVICE* graphicsDeviceCreateDefault(void)
{
#if defined(__APPLE__) && __APPLE__
	return apple_createDefaultGraphicsDevice();
#else
	return NULL;
#endif
}

void graphicsDeviceRelease(GRAPHICS_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	
#if defined(__APPLE__) && __APPLE__
	apple_releaseDefaultGraphicsDevice(graphicsDevice);
#else
	//
#endif
}


const char* graphicsDeviceGetName(GRAPHICS_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	return stringGetData(&graphicsDevice->name);
}

void* graphicsDeviceGetUserInfo(GRAPHICS_DEVICE* graphicsDevice)
{
	assert(graphicsDevice);
	return graphicsDevice->userInfo;
}


TEXTURE2D* graphicsDeviceCreateTexture2d(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, const void* data)
{
	assert(device);
	assert(data);
	
	void* textureId = device->createTexture2dFunc(device, width, height, numComponents, data);
	assert(textureId);
	
	TEXTURE2D* texture = malloc(sizeof(TEXTURE2D));
	texture->device = device;
	texture->width = width;
	texture->height = height;
	texture->textureId = textureId;
	
	return texture;
}

void graphicsDeviceReleaseTexture2d(GRAPHICS_DEVICE* device, TEXTURE2D* texture)
{
	assert(device);
	assert(texture);
	
	device->releaseTexture2dFunc(device, texture->textureId);
	debug_memset(texture, 0, sizeof(TEXTURE2D));
	free(texture);
}
