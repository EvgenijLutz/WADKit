//
//  wad.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//
// Achtung! These structures do not reflect .wad file

#ifndef wad_kit_wad_h
#define wad_kit_wad_h

#include "wk_vertex.h"
#include "wk_polygon.h"
#include "animation.h"
#include "texture_page.h"
#include "movable.h"

typedef struct TEXTURE_SAMPLE TEXTURE_SAMPLE;
typedef struct TEXTURE_PAGE TEXTURE_PAGE;
typedef struct MESH MESH;
typedef struct SKELETON SKELETON;
typedef struct MOVABLE MOVABLE;
typedef struct STATIC STATIC;

typedef struct WAD
{
	unsigned int version;
	
	unsigned int numTextureSamples;
	TEXTURE_SAMPLE* textureSamples;
	
	unsigned int numTexturePages;
	TEXTURE_PAGE* texturePages;
	
	unsigned int numMeshes;
	MESH* meshes;
	
	/* In theory, movables can refer to the same skeleton. Let's check it in future.
	   If not, then move each skeleton instance to related mesh. */
	unsigned int numSkeletons;
	SKELETON* skeletons;
	
	unsigned int numMovables;
	MOVABLE* movables;
	
	unsigned int numStatics;
	STATIC* statics;
}
WAD;

#endif /* wad_kit_wad_h */
