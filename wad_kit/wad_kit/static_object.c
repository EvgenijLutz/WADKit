//
//  static_object.c
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

// MARK: - Private interface

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

// MARK: - Public interface

static char* _staticId_fabulousNames[] =
{
	"Plant 0", "Plant 1", "Plant 2", "Plant 3", "Plant 4",
	"Plant 5", "Plant 6", "Plant 7", "Plant 8", "Plant 9",
	
	"Furniture 0", "Furniture 1", "Furniture 2", "Furniture 3", "Furniture 4",
	"Furniture 5", "Furniture 6", "Furniture 7", "Furniture 8", "Furniture 9",
	
	"Rock 0", "Rock 1", "Rock 2", "Rock 3", "Rock 4",
	"Rock 5", "Rock 6", "Rock 7", "Rock 8", "Rock 9",
	
	"Architecture 0", "Architecture 1", "Architecture 2", "Architecture 3", "Architecture 4",
	"Architecture 5", "Architecture 6", "Architecture 7", "Architecture 8", "Architecture 9",
	
	"Debris 0", "Debris 1", "Debris 2", "Debris 3", "Debris 4",
	"Debris 5", "Debris 6", "Debris 7", "Debris 8", "Debris 9",
	
	"Shatter 0", "Shatter 1", "Shatter 2", "Shatter 3", "Shatter 4",
	"Shatter 5", "Shatter 6", "Shatter 7", "Shatter 8", "Shatter 9",
};

static char* _staticId_terribleNames[] =
{
	"PLANT0", "PLANT1", "PLANT2", "PLANT3", "PLANT4",
	"PLANT5", "PLANT6", "PLANT7", "PLANT8", "PLANT9",
	
	"FURNITURE0", "FURNITURE1", "FURNITURE2", "FURNITURE3", "FURNITURE4",
	"FURNITURE5", "FURNITURE6", "FURNITURE7", "FURNITURE8", "FURNITURE9",
	
	"ROCK0", "ROCK1", "ROCK2", "ROCK3", "ROCK4",
	"ROCK5", "ROCK6", "ROCK7", "ROCK8", "ROCK9",
	
	"ARCHITECTURE0", "ARCHITECTURE1", "ARCHITECTURE2", "ARCHITECTURE3",
	"ARCHITECTURE4", "ARCHITECTURE5", "ARCHITECTURE6", "ARCHITECTURE7",
	"ARCHITECTURE8", "ARCHITECTURE9",
	
	"DEBRIS0", "DEBRIS1", "DEBRIS2", "DEBRIS3", "DEBRIS4",
	"DEBRIS5", "DEBRIS6", "DEBRIS7", "DEBRIS8", "DEBRIS9",
	
	"SHATTER0", "SHATTER1", "SHATTER2", "SHATTER3", "SHATTER4",
	"SHATTER5", "SHATTER6", "SHATTER7", "SHATTER8", "SHATTER9",
};


STATIC_ID staticGetId(STATIC* staticObject)
{
	assert(staticObject);
	return staticObject->staticId;
}

MESH* staticGetMesh(STATIC* staticObject)
{
	assert(staticObject);
	return staticObject->mesh;
}

const char* staticIdGetFabulousName(STATIC_ID staticId)
{
	assert(staticId >= STATIC_ID_PLANT0 && staticId < STATIC_ID_NUMBER_STATIC_OBJECTS);
	return _staticId_fabulousNames[staticId];
}

const char* staticIdGetTerribleName(STATIC_ID staticId)
{
	// Your makeup is terrible
	// But I love anyway
	
	assert(staticId >= STATIC_ID_PLANT0 && staticId < STATIC_ID_NUMBER_STATIC_OBJECTS);
	return _staticId_terribleNames[staticId];
}
