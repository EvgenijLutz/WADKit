//
//  semaphore_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

WK_SEMAPHORE* systemCreateSemaphore(WK_SYSTEM* system, int value)
{
	assert(system);
	
	WK_SEMAPHORE* semaphore = malloc(sizeof(semaphore));
	semaphore->system = system;
	semaphore->semaphoreId = system->createSemaphoreFunc(system, value, system->userInfo);
	return semaphore;
}

void semaphoreRelease(WK_SEMAPHORE* semaphore)
{
	assert(semaphore);
	
	semaphore->system->releaseSemaphoreFunc(semaphore->system, semaphore, semaphore->system->userInfo);
	debug_memset(semaphore, 0, sizeof(WK_SEMAPHORE));
	free(semaphore);
}


void semaphoreEnter(WK_SEMAPHORE* semaphore)
{
	assert(semaphore);
	semaphore->system->semaphoreEnterFunc(semaphore->system, semaphore->semaphoreId, semaphore->system->userInfo);
}

void semaphoreLeave(WK_SEMAPHORE* semaphore)
{
	assert(semaphore);
	semaphore->system->semaphoreLeaveFunc(semaphore->system, semaphore->semaphoreId, semaphore->system->userInfo);
}
