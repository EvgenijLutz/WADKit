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
	RESOURCE_STORAGE* resourceStorage;
	
	char* filePath;
	WK_WAD* wad;
	
	WAD_EDITOR_ITEM* rootItem;
	WAD_EDITOR_ITEM* texturePagesItem;
	WAD_EDITOR_ITEM* meshesItem;
	WAD_EDITOR_ITEM* skeletonsItem;
	WAD_EDITOR_ITEM* movablesItem;
	WAD_EDITOR_ITEM* staticsItem;
	
	WAD_EDITOR_ITEM* selectedItem;
	
	WAD_EDITOR_VIEWPORT_DELEGATE* meshViewportDelegate;
	WAD_EDITOR_VIEWPORT* mainViewport;
	
	// TODO: undo/redo buffer
}
WAD_EDITOR;

#endif /* wad_editor_lib__wad_editor_h */
