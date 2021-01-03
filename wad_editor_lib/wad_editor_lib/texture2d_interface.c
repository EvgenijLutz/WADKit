//
//  texture2d_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void texture2dRelease(TEXTURE2D* texture)
{
	assert(texture);
	graphicsDeviceReleaseTexture2d(texture->device, texture);
}
