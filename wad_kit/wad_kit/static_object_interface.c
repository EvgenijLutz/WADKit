//
//  static_object_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

char* staticIdNames[] =
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

const char* staticIdGetName(STATIC_ID staticId)
{
	assert(staticId >= STATIC_ID_PLANT0 && staticId < STATIC_ID_NUMBER_STATIC_OBJECTS);
	return staticIdNames[staticId];
}
