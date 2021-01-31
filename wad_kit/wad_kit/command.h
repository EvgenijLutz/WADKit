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

typedef struct WK_COMMAND_POSITION
{
	short x;
	short y;
	short z;
}
WK_COMMAND_POSITION;

typedef struct WK_COMMAND_JUMP
{
	short xy;
	short yz;
}
WK_COMMAND_JUMP;

typedef struct WK_COMMAND_PLAY_SOUND
{
	unsigned short frame;
	unsigned short soundID;
}
WK_COMMAND_PLAY_SOUND;

typedef struct WK_COMMAND_ACTION
{
	unsigned short frame;
	unsigned short action;
}
WK_COMMAND_ACTION;

typedef struct WK_COMMAND
{
	WK_COMMAND_TYPE commandType;
	union
	{
		WK_COMMAND_POSITION positionCommand;
		WK_COMMAND_JUMP jumpCommand;
		WK_COMMAND_PLAY_SOUND playSoundCommand;
		WK_COMMAND_ACTION actionCommand;
	} parameters;
}
WK_COMMAND;

#endif /* command_h */
