//
//  wad_editor_item_type.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 16.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_editor_lib__wad_editor_item_type_h
#define wad_editor_lib__wad_editor_item_type_h

typedef enum WAD_EDITOR_ITEM_TYPE
{
	WAD_EDITOR_ITEM_TYPE_ROOT = 0,
	WAD_EDITOR_ITEM_TYPE_SECTION,
	
	// Texture
	WAD_EDITOR_ITEM_TYPE_TEXTURE,
	
	// Mesh
	WAD_EDITOR_ITEM_TYPE_MESH,
	
	// Skeleton
	WAD_EDITOR_ITEM_TYPE_SKELETON,
	WAD_EDITOR_ITEM_TYPE_SKELETON_JOINT,
	
	// Movable
	WAD_EDITOR_ITEM_TYPE_MOVABLE,
	WAD_EDITOR_ITEM_TYPE_MOVABLE_ROOT_MESH,
	// Movable meshes
	WAD_EDITOR_ITEM_TYPE_MOVABLE_MESHES,
	WAD_EDITOR_ITEM_TYPE_MOVABLE_MESH,
	// Movable skeleton
	WAD_EDITOR_ITEM_TYPE_MOVABLE_SKELETON,
	WAD_EDITOR_ITEM_TYPE_MOVABLE_SKELETON_JOINT,
	// Movable animations
	WAD_EDITOR_ITEM_TYPE_MOVABLE_ANIMATIONS,
	WAD_EDITOR_ITEM_TYPE_MOVABLE_ANIMATION,
	
	// Static
	WAD_EDITOR_ITEM_TYPE_STATIC,
}
WAD_EDITOR_ITEM_TYPE;

#endif /* wad_editor_lib__wad_editor_item_type_h */
