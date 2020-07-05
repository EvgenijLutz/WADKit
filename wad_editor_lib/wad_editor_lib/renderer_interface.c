//
//  renderer_interface.c
//  wad_editor_lib
//
//  Created by Евгений Лютц on 07.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "renderer_interface.h"
#include "renderer.h"
#include "common.h"

RENDERER* rendererCreate(RESOURCE_STORAGE* resourceStorage, RENDERER_DELEGATE_DRAW_MESH_FUNC drawMeshFunc, RENDERER_DELEGATE_DRAW_DUMMY_CUBE_FUNC drawDummyCubeFunc, void* userInfo)
{
	assert(resourceStorage);
	assert(drawMeshFunc);
	assert(drawDummyCubeFunc);
	
	RENDERER* renderer = malloc(sizeof(RENDERER));
	renderer->resourceStorage = resourceStorage;
	renderer->drawMeshFunc = drawMeshFunc;
	renderer->drawDummyCubeFunc = drawDummyCubeFunc;
	renderer->userInfo = userInfo;
	
	return renderer;
}

void rendererRelease(RENDERER* renderer)
{
	assert(renderer);
	free(renderer);
}

void rendererRenderMesh(RENDERER* renderer, MESH* mesh, matrix4f* model, matrix4f* view, matrix4f* projection)
{
	assert(renderer);
	renderer->drawMeshFunc(mesh, model, view, projection, renderer->userInfo);
}

void rendererRenderDummyCube(RENDERER* renderer, matrix4f* model, matrix4f* view, matrix4f* projection)
{
	renderer->drawDummyCubeFunc(model, view, projection, renderer->userInfo);
}
