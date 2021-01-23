//
//  raw_wad.h
//  wad_kit
//
//  Created by Евгений Лютц on 07.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_raw_wad_h
#define wad_kit_raw_wad_h

#include "private_interface.h"

typedef struct __attribute__((__packed__)) RAW_ANIMATION
{
	uint32_t keyframeOffset;
	uint8_t frameDuration;
	uint8_t keyframeSize;
	uint16_t stateId;
	int16_t unknown1;
	int16_t speed;
	int32_t acceleration;
	int64_t unknown2;
	uint16_t frameStart;
	uint16_t frameEnd;
	uint16_t nextAnimation;
	uint16_t frameIn;
	uint16_t numStateChanges;
	uint16_t changesIndex;
	uint16_t numCommands;
	uint16_t commandOffsets;
}
RAW_ANIMATION;

typedef struct __attribute__((__packed__)) RAW_STATE_CHANGE
{
	uint16_t stateId;
	uint16_t numDispatches;
	uint16_t dispatchesIndex;
}
RAW_STATE_CHANGE;

typedef struct __attribute__((__packed__)) RAW_DISPATCH
{
	uint16_t inRange;
	uint16_t outRange;
	uint16_t nextAnim;
	uint16_t frameIn;
}
RAW_DISPATCH;

typedef struct __attribute__((__packed__)) RAW_LINK
{
	int32_t opCode;
	int32_t dx;
	int32_t dy;
	int32_t dz;
}
RAW_LINK;

typedef struct __attribute__((__packed__)) RAW_KEYFRAME
{
	int16_t bb1x;
	int16_t bb2x;
	int16_t bb1y;
	int16_t bb2y;
	int16_t bb1z;
	int16_t bb2z;
	int16_t offx;
	int16_t offy;
	int16_t offz;
}
RAW_KEYFRAME;

typedef struct __attribute__((__packed__)) RAW_MOVABLE
{
	uint32_t objectId;
	uint16_t numPointers;
	uint16_t pointersIndex;
	uint32_t linksIndex;
	uint32_t keyframesOffset;
	int16_t animsIndex;
}
RAW_MOVABLE;

typedef struct __attribute__((__packed__)) RAW_STATIC
{
	uint32_t objectId;
	uint16_t pointersIndex;
	int16_t vx1;
	int16_t vx2;
	int16_t vy1;
	int16_t vy2;
	int16_t vz1;
	int16_t vz2;
	int16_t cx1;
	int16_t cx2;
	int16_t cy1;
	int16_t cy2;
	int16_t cz1;
	int16_t cz2;
	uint16_t flags;
}
RAW_STATIC;

#endif /* wad_kit_raw_wad_h */
