//
//  renderer_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__renderer_interface_h
#define wad_editor_lib__renderer_interface_h

#include "wad_editor_lib.h"

RENDERER* rendererCreate(RESOURCE_STORAGE* resourceStorage, RENDERER_DELEGATE_DRAW_MESH_FUNC drawMeshFunc, RENDERER_DELEGATE_DRAW_DUMMY_CUBE_FUNC drawDummyCubeFunc, void* userInfo);
void rendererRelease(RENDERER* renderer);

void rendererRenderMesh(RENDERER* renderer, MESH* mesh, matrix4f* model, matrix4f* view, matrix4f* projection);
void rendererRenderDummyCube(RENDERER* renderer, matrix4f* model, matrix4f* view, matrix4f* projection);

#endif /* wad_editor_lib__renderer_interface_h */
