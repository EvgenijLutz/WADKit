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


TEXTURE2D* graphicsDeviceCreateTexture2d(GRAPHICS_DEVICE* device, unsigned int width, unsigned int height, unsigned int numComponents, TEXTURE_USAGE usage, const void* data)
{
	assert(device);
	assert(data);
	
	void* textureId = device->createTexture2dFunc(device, width, height, numComponents, usage, data);
	assert(textureId);
	
	TEXTURE2D* texture = malloc(sizeof(TEXTURE2D));
	texture->device = device;
	texture->width = width;
	texture->height = height;
	texture->textureId = textureId;
	
	return texture;
}

GRAPHICS_BUFFER* graphicsDeviceCreateBuffer(GRAPHICS_DEVICE* device, unsigned int length, GRAPHICS_BUFFER_OPTION option)
{
	assert(device);
	
	void* bufferId = device->createBufferFunc(device, length, option);
	assert(bufferId);
	
	GRAPHICS_BUFFER* buffer = arrayAddItem(&device->buffers);
	buffer->device = device;
	buffer->length = length;
	buffer->option = option;
	buffer->bufferId = bufferId;
	return buffer;
}

COMMAND_QUEUE* graphicsDeviceCreateCommandQueue(GRAPHICS_DEVICE* device)
{
	assert(device);
	
	void* commandQueueId = device->createCommandQueueFunc(device);
	assert(commandQueueId);
	
	COMMAND_QUEUE* commandQueue = malloc(sizeof(COMMAND_QUEUE));
	commandQueue->device = device;
	commandQueue->commandQueueId = commandQueueId;
	return commandQueue;
}
