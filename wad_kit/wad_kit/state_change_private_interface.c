//
//  state_change_private_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void stateChangeInitialize(STATE_CHANGE* stateChange, ANIMATION* animation, RAW_STATE_CHANGE* rawStateChange, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(stateChange);
	
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	stateChange->animation = animation;
	stateChange->stateId = rawStateChange->stateId;
	arrayInitialize(&stateChange->dispatches, sizeof(DISPATCH));
	
	for (unsigned int i = 0; i < rawStateChange->numDispatches; i++)
	{
		RAW_DISPATCH* rawDispatch = &loadInfo->rawDispatches[rawStateChange->dispatchesIndex + i];
		DISPATCH* dispatch = arrayAddItem(&stateChange->dispatches);
		dispatch->inRange = rawDispatch->inRange;
		dispatch->outRange = rawDispatch->outRange;
		dispatch->nextAnim = rawDispatch->nextAnim;
		dispatch->frameIn = rawDispatch->frameIn;
	}
	
	executeResultSetSucceeded(executeResult);
}

void stateChangeDeinitialize(STATE_CHANGE* stateChange)
{
	assert(stateChange);
	arrayDeinitialize(&stateChange->dispatches);
}
