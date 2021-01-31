//
//  command.c
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void commandInitialize(WK_COMMAND* command, WK_ANIMATION* animation, WK_WAD_LOAD_INFO* loadInfo)
{
	assert(command);
	
	BUFFER_READER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	command->commandType = (WK_COMMAND_TYPE)bufferReaderReadUInt16(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	if (command->commandType == 1)
	{
		command->parameters.positionCommand.x = bufferReaderReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.positionCommand.y = bufferReaderReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.positionCommand.z = bufferReaderReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	else if (command->commandType == 2)
	{
		command->parameters.jumpCommand.xy = bufferReaderReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.jumpCommand.yz = bufferReaderReadInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	else if (command->commandType == 5)
	{
		command->parameters.playSoundCommand.frame = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.playSoundCommand.soundID = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	else if (command->commandType == 6)
	{
		command->parameters.actionCommand.frame = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		command->parameters.actionCommand.action = bufferReaderReadUInt16(buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	executeResultSetSucceeded(executeResult);
}

void commandDeinitialize(WK_COMMAND* command)
{
	assert(command);
	//
}
