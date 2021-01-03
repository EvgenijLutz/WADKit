//
//  graphics_device.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_h
#define graphics_device_h

#include "private_interface.h"

typedef struct GRAPHICS_DEVICE
{
	WK_STRING name;
	
	GRAPHICS_DEVICE_CREATE_TEXTURE2D_FUNC createTexture2dFunc;
	GRAPHICS_DEVICE_RELEASE_TEXTURE2D_FUNC releaseTexture2dFunc;
	
	int createdByDefault;
	void* userInfo;
}
GRAPHICS_DEVICE;

#endif /* graphics_device_h */
