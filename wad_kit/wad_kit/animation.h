//
//  animation.h
//  wad_kit
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef animation_h
#define animation_h

typedef struct ROTATION
{
	unsigned short rotx;
	unsigned short roty;
	unsigned short rotz;
}
ROTATION;

typedef struct KEYFRAME
{
	short bb1x;
	short bb1y;
	short bb1z;
	
	short bb2x;
	short bb2y;
	short bb2z;
	
	short offx;
	short offy;
	short offz;
	
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

typedef struct ANIMATION
{
	unsigned short stateId;
	unsigned short numKeyframes;
	KEYFRAME* keyframes;
	unsigned char frameDuration;
	short unknown1;
	short speed;
	int acceleration;
	long unknown2;
	unsigned short frameStart;
	unsigned short frameEnd;
	unsigned short nextAnimation;
	unsigned short nextAnimationFrame;
	unsigned short numStateChanges;
	STATE_CHANGE* stateChanges;
	unsigned short numCommands;
	COMMAND* commands;
}
ANIMATION;

typedef struct JOINT
{
	int opCode;
	int dx;
	int dy;
	int dz;
}
JOINT;

/// Skeleton information. Consists of skeleton joints except root joint.
typedef struct SKELETON_INFO
{
	unsigned int numJoints;
	JOINT* joints;
}
SKELETON;

#endif /* animation_h */
