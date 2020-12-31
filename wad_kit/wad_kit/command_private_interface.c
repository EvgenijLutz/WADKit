//
//  command_private_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void commandInitialize(COMMAND* command, ANIMATION* animation, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(command);
	
	WK_BUFFER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	command->commandType = (WK_COMMAND_TYPE)bufferReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	if (command->commandType == 1)
	{
		command->parameters.positionCommand.x = bufferReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.positionCommand.y = bufferReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.positionCommand.z = bufferReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	else if (command->commandType == 2)
	{
		command->parameters.jumpCommand.xy = bufferReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.jumpCommand.yz = bufferReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	else if (command->commandType == 5)
	{
		command->parameters.playSoundCommand.frame = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.playSoundCommand.soundID = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	else if (command->commandType == 6)
	{
		command->parameters.actionCommand.frame = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.actionCommand.action = bufferReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	executeResultSetSucceeded(executeResult);
}

void commandDeinitialize(COMMAND* command)
{
	assert(command);
	//
}
