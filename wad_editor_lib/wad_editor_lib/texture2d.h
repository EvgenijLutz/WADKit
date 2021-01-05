//
//  texture2d.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef texture2d_h
#define texture2d_h

#include "private_interface.h"

typedef struct TEXTURE2D
{
	GRAPHICS_DEVICE* device;
	
	unsigned int width;
	unsigned int height;
	TEXTURE_USAGE usage;
	
	/// It means that it's not created manually, but, for example, received from system.
	int isReceivedOutside;
	void* textureId;
}
TEXTURE2D;

#endif /* texture2d_h */
