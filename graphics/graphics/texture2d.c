//
//  texture2d.c
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

TEXTURE2D* graphicsDeviceCreateTexture2dWithData(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, TEXTURE_USAGE usage, const void* data)
{
	assert(device);
	assert(data);
	
	void* textureId = device->createTexture2dWithDataFunc(device, width, height, numComponents, usage, data);
	assert(textureId);
	
	TEXTURE2D* texture = magicArrayAddItem(&device->textures);
	texture->device = device;
	texture->width = width;
	texture->height = height;
	texture->isReceivedOutside = 0;
	texture->textureId = textureId;
	
	return texture;
}

void texture2dRelease(TEXTURE2D* texture)
{
	assert(texture);
	
	GRAPHICS_DEVICE* device = texture->device;
	device->releaseTexture2dFunc(device, texture->textureId);
	debug_memset(texture, 0, sizeof(TEXTURE2D));
	magicArrayRemoveItem(&device->textures, texture);
}
