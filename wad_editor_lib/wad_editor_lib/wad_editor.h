//
//  wad_editor.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_h
#define wad_editor_lib__wad_editor_h

#include "wad_editor_lib.h"

typedef struct WAD_EDITOR
{
	WK_SYSTEM* system;
	
	WK_STRING* openedWadPath;
	WK_WAD* wad;
	int hasChanges;
	
	GRAPHICS_VIEW* outputView;
	GRAPHICS_DEVICE* device;
	
	// TODO: undo/redo buffer
}
WAD_EDITOR;

#endif /* wad_editor_lib__wad_editor_h */
