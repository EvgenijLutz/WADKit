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

typedef struct TRANSFORM_DATA
{
	matrix4f modelMatrix;	// sch💩ße
	unsigned int stackParentIndex;
	
	MESH_DATA* mesh;
	MESH_DATA* skinJointMesh;
	GRAPHICS_MESH_UNIFORMS* uniforms;
}
TRANSFORM_DATA;

typedef struct WAD_EDITOR
{
	SYSTEM* system;
	SEMAPHORE* accessSemaphore;
	
	STRING* openedWadPath;
	WK_WAD* wad;
	int hasChanges;
	
	WE_LIST* wadContentsList;
	GRAPHICS_VIEW* outputView;
	
	GRAPHICS_DEVICE* device;
	COMMAND_QUEUE* commandQueue;
	
	double lastUpdateTime;
	
	
	MAGIC_ARRAY textures;	// Array of TEXTURE_DATA
	
	DATA_ALLOCATOR* submeshAllocator;	// Allocates SUBMESH_DATA. Used by WAD_EDITOR.meshes
	MAGIC_ARRAY meshes;					// Array of MESH_DATA
	
	DATA_ALLOCATOR* movableJointsAllocator;	// Allocates MOVABLE_JOINT_DATA
	MAGIC_ARRAY movables;
	
	MAGIC_ARRAY animations;	// Array of ANIMATION_DATA
	
	MAGIC_ARRAY statics;
	
	
	TEXTURE_DATA* selectedTexture;
	MESH_DATA* selectedMesh;
	MOVABLE_DATA* selectedMovable;
	STATIC_DATA* selectedStatic;
	
	unsigned int numTranslations;
	unsigned int translationsCapacity;
	TRANSFORM_DATA* transforms;
	
	ANIMATION* selectedAnimation;
	float animationTime;
	
	int useInterpolation;
	int useQuaternionRotations;
	
	// TODO: undo/redo buffer
}
WAD_EDITOR;

#endif /* wad_editor_lib__wad_editor_h */
