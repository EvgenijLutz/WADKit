//
//  command.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef command_h
#define command_h

#include "private_interface.h"


typedef struct POSITION_COMMAND
{
	short x;
	short y;
	short z;
}
POSITION_COMMAND;

typedef struct JUMP_COMMAND
{
	short xy;
	short yz;
}
JUMP_COMMAND;

typedef struct PLAY_SOUND_COMMAND
{
	unsigned short frame;
	unsigned short soundID;
}
PLAY_SOUND_COMMAND;

typedef struct ACTION_COMMAND
{
	unsigned short frame;
	unsigned short action;
}
ACTION_COMMAND;

typedef struct COMMAND
{
	WK_COMMAND_TYPE commandType;
	union
	{
		POSITION_COMMAND positionCommand;
		JUMP_COMMAND jumpCommand;
		PLAY_SOUND_COMMAND playSoundCommand;
		ACTION_COMMAND actionCommand;
	} parameters;
}
COMMAND;

#endif /* command_h */
