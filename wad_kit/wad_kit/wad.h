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

#include "private_interface.h"

typedef struct WK_WAD_LOAD_INFO
{
	WK_WAD* wad;
	WK_BUFFER* buffer;
	EXECUTE_RESULT* executeResult;
	
	unsigned int numMeshPointers;
	unsigned int meshPointersDataSize;
	unsigned long meshPointersDataLocation;
	unsigned int* meshPointers;
	
	unsigned int numMeshDataOffsets;
	unsigned int* meshDataOffsets;
	
	unsigned long meshDataSize;
	unsigned long meshDataLocation;
	
	unsigned int numAnimations;
	unsigned long animationsDataSize;
	unsigned long animationsDataLocation;
	RAW_ANIMATION* rawAnimations;
	
	unsigned long stateChangesDataSize;
	unsigned long stateChangesDataLocation;
	RAW_STATE_CHANGE* rawStateChanges;
	
	unsigned long dispatchesDataSize;
	unsigned long dispatchesDataLocation;
	RAW_DISPATCH* rawDispatches;
	
	unsigned long commandsDataSize;
	unsigned long commandsDataLocation;
	
	unsigned long linksDataSize;
	unsigned long linksDataLocation;
	
	unsigned int numKeyframesWords;
	unsigned long keyframesDataSize;
	unsigned long keyframesDataLocation;
	
	unsigned int numMovables;
	unsigned long movablesDataSize;
	unsigned long movablesDataLocation;
	RAW_MOVABLE* rawMovables;
	
	unsigned int numStatics;
	unsigned long staticsDataSize;
	unsigned long staticsDataLocation;
	RAW_STATIC* rawStatics;
}
WK_WAD_LOAD_INFO;

typedef struct WK_WAD
{
	unsigned int version;
	
	/*!
	 Array of @b TEXTURE_PAGE elements.
	 */
	WK_ARRAY texturePages;
	
	/*!
	 Array of @b TEXTURE_SAMPLE elements.
	 */
	WK_ARRAY textureSamples;
	
	/*!
	 Array of @b WK_ARRAY elements.
	 */
	WK_ARRAY meshes;
	
	/*!
	 Array of @b SKELETON elements.
	 In theory, movables can refer to the same skeleton. Let's check it in future.
	 If not, then move each skeleton instance to related movable.
	 No, we will not doing it, it's just a headache.
	 */
	//WK_ARRAY skeletons;
	
	/*!
	 Array of @b MOVABLE elements.
	 */
	WK_ARRAY movables;
	
	/*!
	 Array of @b STATIC elements.
	 */
	WK_ARRAY statics;
}
WK_WAD;

#endif /* wad_kit_wad_h */
