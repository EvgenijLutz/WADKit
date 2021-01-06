//
//  texture2d_interface.c
//  graphics
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void texture2dRelease(TEXTURE2D* texture)
{
	assert(texture);
	
	GRAPHICS_DEVICE* device = texture->device;
	device->releaseTexture2dFunc(device, texture->textureId);
	debug_memset(texture, 0, sizeof(TEXTURE2D));
	arrayRemoveItem(&device->textures, texture);
}
