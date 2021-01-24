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

typedef struct MESH_DATA MESH_DATA;

typedef struct WAD_EDITOR
{
	WK_SYSTEM* system;
	
	WK_STRING* openedWadPath;
	WK_WAD* wad;
	int hasChanges;
	
	WE_LIST* wadContentsList;
	GRAPHICS_VIEW* outputView;
	
	GRAPHICS_DEVICE* device;
	COMMAND_QUEUE* commandQueue;
	
	WK_SEMAPHORE* accessSemaphore;
	double lastUpdateTime;
	
	WK_ARRAY textures;	// Array of TEXTURE_LINK
	
	DATA_ALLOCATOR* submeshAllocator;	// Allocates SUBMESH_DATA. Used by WAD_EDITOR.meshes
	WK_ARRAY meshes;					// Array of MESH_DATA
	
	MESH_DATA* selectedMesh;
	
	GRAPHICS_MESH_UNIFORMS* testMeshUniforms;
	
	// TODO: undo/redo buffer
}
WAD_EDITOR;

#endif /* wad_editor_lib__wad_editor_h */
