//
//  wad_private_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

MESH* _wad_findMesh(unsigned short pointerListStart, unsigned short index, WK_WAD_LOAD_INFO* loadInfo)
{
	if (pointerListStart + index >= loadInfo->numMeshPointers)
	{
		executeResultSetFailed(loadInfo->executeResult, "Cannot find mesh. Invalid index to data is given.");
		return NULL;
	}
	
	unsigned int value = loadInfo->meshPointers[pointerListStart + index];
	
	for (unsigned int i = 0; i < loadInfo->numMeshDataOffsets; i++)
	{
		if (loadInfo->meshDataOffsets[i] == value)
		{
			return wadGetMesh(loadInfo->wad, i);
		}
	}
	
	executeResultSetFailed(loadInfo->executeResult, "Cannot find mesh.");
	return NULL;
}
