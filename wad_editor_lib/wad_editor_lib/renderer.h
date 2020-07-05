//
//  renderer.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__renderer_h
#define wad_editor_lib__renderer_h

#include "wad_kit_link.h"
#include "renderer_functions.h"

typedef struct RESOURCE_STORAGE RESOURCE_STORAGE;

typedef struct RENDERER
{
	RESOURCE_STORAGE* resourceStorage;
	
	RENDERER_DELEGATE_DRAW_MESH_FUNC drawMeshFunc;
	RENDERER_DELEGATE_DRAW_DUMMY_CUBE_FUNC drawDummyCubeFunc;
	
	void* userInfo;
}
RENDERER;

#endif /* wad_editor_lib__renderer_h */
