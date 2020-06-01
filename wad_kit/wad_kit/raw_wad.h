//
//  raw_wad.h
//  wad_kit
//
//  Created by Евгений Лютц on 07.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_raw_wad_h
#define wad_kit_raw_wad_h

typedef struct RAW_ANIMATION
{
	unsigned int keyframeOffset;
	unsigned char frameDuration;
	unsigned char keyframeSize;
	unsigned short stateId;
	short unknown1;
	short speed;
	int acceleration;
	long unknown2;
	unsigned short frameStart;
	unsigned short frameEnd;
	unsigned short nextAnimation;
	unsigned short frameIn;
	unsigned short numStateChanges;
	unsigned short changesIndex;
	unsigned short numCommands;
	unsigned short commandOffsets;
}
RAW_ANIMATION;

typedef struct RAW_STATE_CHANGE
{
	unsigned short stateId;
	unsigned short numDispatches;
	unsigned short dispatchesIndex;
}
RAW_STATE_CHANGE;

typedef struct RAW_DISPATCH
{
	unsigned short inRange;
	unsigned short outRange;
	unsigned short nextAnim;
	unsigned short frameIn;
}
RAW_DISPATCH;

typedef struct RAW_LINK
{
	int opCode;
	int dx;
	int dy;
	int dz;
}
RAW_LINK;

typedef struct RAW_KEYFRAME
{
	short bb1x;
	short bb2x;
	short bb1y;
	short bb2y;
	short bb1z;
	short bb2z;
	short offx;
	short offy;
	short offz;
	// keys
}
RAW_KEYFRAME;

typedef struct RAW_MOVABLE
{
	unsigned int objectId;
	unsigned short numPointers;
	unsigned short pointersIndex;
	unsigned int linksIndex;
	unsigned int keyframesOffset;
	short animsIndex;
}
RAW_MOVABLE;

typedef struct RAW_STATIC
{
	unsigned int objectId;
	unsigned short pointersIndex;
	short vx1;
	short vx2;
	short vy1;
	short vy2;
	short vz1;
	short vz2;
	short cx1;
	short cx2;
	short cy1;
	short cy2;
	short cz1;
	short cz2;
	unsigned short flags;
}
RAW_STATIC;

typedef struct RAW_WAD
{
	//
}
RAW_WAD;

#endif /* wad_kit_raw_wad_h */
