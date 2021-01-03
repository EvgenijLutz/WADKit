//
//  renderer.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__renderer_h
#define wad_editor_lib__renderer_h

#include "wad_editor_lib.h"

typedef struct RENDERER
{
	RESOURCE_STORAGE* resourceStorage;
	
	RENDERER_DELEGATE_DRAW_MESH_FUNC drawMeshFunc;
	RENDERER_DELEGATE_DRAW_DUMMY_CUBE_FUNC drawDummyCubeFunc;
	
	void* userInfo;
}
RENDERER;

#endif /* wad_editor_lib__renderer_h */
