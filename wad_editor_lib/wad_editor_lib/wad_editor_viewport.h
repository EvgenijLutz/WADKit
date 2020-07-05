//
//  wad_editor_viewport.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_viewport_h
#define wad_editor_lib__wad_editor_viewport_h

#include "wad_kit_link.h"
#include "we_mouse_key.h"

typedef struct WAD_EDITOR WAD_EDITOR;
typedef struct WAD_EDITOR_VIEWPORT_DELEGATE WAD_EDITOR_VIEWPORT_DELEGATE;
typedef struct RENDERER RENDERER;

typedef struct WAD_EDITOR_VIEWPORT
{
	WAD_EDITOR* editor;
	
	WAD_EDITOR_VIEWPORT_DELEGATE* delegate;
	RENDERER* renderer;
	
	vector2f size;
	
	// Mouse stashed data
	vector2f mouseDelta;
	WE_MOUSE_KEY keys[WE_MOUSE_KEY_OTHER + 1];
}
WAD_EDITOR_VIEWPORT;

#endif /* wad_editor_lib__wad_editor_viewport_h */
