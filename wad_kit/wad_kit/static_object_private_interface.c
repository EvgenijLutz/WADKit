//
//  static_object_private_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

static void _static_updateVisibilityBox(STATIC* staticObject)
{
	//
}

static void _static_updateCollisionBox(STATIC* staticObject)
{
	//
}

void staticInitialize(STATIC* staticObject, RAW_STATIC* rawStatic, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(staticObject);
	
	WK_WAD* wad = loadInfo->wad;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	staticObject->wad = wad;
	staticObject->staticId = (STATIC_ID)rawStatic->objectId;
	staticObject->mesh = _wad_findMesh(rawStatic->pointersIndex, 0, loadInfo);
	if (executeResultIsFailed(executeResult)) { return; }
	staticObject->mesh->numReferences++;
	
	staticObject->vx1 = rawStatic->vx1;
	staticObject->vy1 = rawStatic->vy1;
	staticObject->vz1 = rawStatic->vz1;
	staticObject->vx2 = rawStatic->vx2;
	staticObject->vy2 = rawStatic->vy2;
	staticObject->vz2 = rawStatic->vz2;
	_static_updateVisibilityBox(staticObject);
	
	staticObject->cx1 = rawStatic->cx1;
	staticObject->cy1 = rawStatic->cy1;
	staticObject->cz1 = rawStatic->cz1;
	staticObject->cx2 = rawStatic->cx2;
	staticObject->cy2 = rawStatic->cy2;
	staticObject->cz2 = rawStatic->cz2;
	_static_updateCollisionBox(staticObject);
	
	staticObject->flags = rawStatic->flags;
	
	executeResultSetSucceeded(executeResult);
}

void staticDeinitialize(STATIC* staticObject)
{
	assert(staticObject);
	staticObject->mesh->numReferences--;
}
