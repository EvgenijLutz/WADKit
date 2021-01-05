//
//  graphics_drawable.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_drawable_h
#define graphics_drawable_h

#include "private_interface.h"

typedef struct GRAPHICS_DRAWABLE
{
	GRAPHICS_VIEW* graphicsView;
	
	void* drawableId;
	
	TEXTURE2D* texture;
}
GRAPHICS_DRAWABLE;

#endif /* graphics_drawable_h */
