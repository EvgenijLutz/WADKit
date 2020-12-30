//
//  keyframe.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef keyframe_h
#define keyframe_h

#include "private_interface.h"

typedef struct ROTATION
{
	unsigned short rotx;
	unsigned short roty;
	unsigned short rotz;
	
	vector3f rotation;
}
ROTATION;

typedef struct KEYFRAME
{
	ANIMATION* animation;
	
	short bb1x;
	short bb1y;
	short bb1z;
	
	short bb2x;
	short bb2y;
	short bb2z;
	
	// Root mesh offset
	short offx;
	short offy;
	short offz;
	
	vector3f boundingBoxStart;
	vector3f boundingBoxEnd;
	vector3f modelOffset;
	
	/* Number of rotations is always equal to object's number of meshes */
	ROTATION* rotations;
}
KEYFRAME;



typedef struct DISPATCH
{
	unsigned short inRange;
	unsigned short outRange;
	unsigned short nextAnim;
	unsigned short frameIn;
}
DISPATCH;

typedef struct STATE_CHANGE
{
	unsigned short stateId;
	unsigned short numDispatches;
	DISPATCH* dispatches;
}
STATE_CHANGE;



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
	unsigned short commandType;
	union
	{
		POSITION_COMMAND positionCommand;
		JUMP_COMMAND jumpCommand;
		PLAY_SOUND_COMMAND playSoundCommand;
		ACTION_COMMAND actionCommand;
	} parameters;
}
COMMAND;

#endif /* keyframe_h */
