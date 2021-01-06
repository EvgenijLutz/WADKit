//
//  system_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

#if defined(__APPLE__) && __APPLE__
#include "system_apple.h"
#endif


WK_SYSTEM* systemCreateDefaultIfAvailable(void)
{
#if defined(__APPLE__) && __APPLE__
	return systemCreateDefaultApple();
#else
	return NULL;
#endif
}

void systemRelease(WK_SYSTEM* system)
{
	assert(system);
	
	if (system->createdDefault)
	{
#if defined(__APPLE__) && __APPLE__
		systemReleaseDefaultApple(system);
#else
		return;
#endif
	}
	else
	{
		free(system);
	}
}

void systemGetPathForResourceWithName(WK_SYSTEM* system, WK_STRING* outPath, const char* name, const char* type, EXECUTE_RESULT* executeResult)
{
	assert(system);
	assert(outPath);
	assert(name);
	assert(type);
	
	system->getResourceFilePathFunc(system, outPath, name, type, executeResult);
}

double systemGetCurrentTime(WK_SYSTEM* system)
{
	assert(system);
	return system->getCurrentTimeFunc();
}
