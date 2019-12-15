//
//  execute_result.c
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "execute_result.h"
#include <string.h>

void executeResultSucceeded(EXECUTE_RESULT* executeResult)
{
	if (executeResult)
	{
		executeResult->succeeded = 1;
		executeResult->message[0] = 0;
	}
}

void executeResultFailed(EXECUTE_RESULT* executeResult, const char* message)
{
	if (executeResult)
	{
		executeResult->succeeded = 0;
		memcpy(executeResult->message, message, strlen(message));
	}
}

void executeResultFailedCopy(EXECUTE_RESULT* executeResult, EXECUTE_RESULT* source)
{
	if (executeResult && source)
	{
		*executeResult = *source;
	}
}
