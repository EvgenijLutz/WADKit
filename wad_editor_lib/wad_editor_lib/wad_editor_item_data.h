//
//  wad_editor_item_data.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 16.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_item_data_h
#define wad_editor_lib__wad_editor_item_data_h

#include "wad_kit_link.h"

typedef union WAD_EDITOR_ITEM_DATA
{
	TEXTURE_PAGE* texturePage;
	
	MESH* mesh;
	
	SKELETON* skeleton;
	struct {
		SKELETON* skeleton;
		JOINT* joint;
	} skeletonJoint;
	
	MOVABLE* movable;
	struct {
		MOVABLE* movable;
		unsigned int movableMeshIndex;
		MESH* mesh;
	} movableRootMesh;
	struct {
		MOVABLE* movable;
		SKELETON* skeleton;
	} movableSkeleton;
	struct {
		MOVABLE* movable;
		SKELETON* skeleton;
		JOINT* joint;
	} movableSkeletonJoint;
	struct {
		MOVABLE* movable;
		ANIMATION* animation;
	} movableAnimation;
	
	STATIC* staticObject;
}
WAD_EDITOR_ITEM_DATA;

#endif /* wad_editor_lib__wad_editor_item_data_h */
