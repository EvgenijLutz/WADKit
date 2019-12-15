//
//  wad.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//
/// These structures do not reflect .wad file

#ifndef wad_h
#define wad_h

typedef struct TEXTURE_SAMPLE
{
	unsigned char x;
	unsigned char y;
	unsigned short page;
	char flipX;
	unsigned char addW;
	char flipY;
	unsigned char addH;
}
TEXTURE_SAMPLE;

typedef struct TEXTURE_PAGE
{
	unsigned char data[256 * 256 * 3];
}
TEXTURE_PAGE;

typedef struct BOUNDING_SPHERE
{
	short cx;
	short cy;
	short cz;
	unsigned short radius;
	unsigned short unknown;	// garbage?
}
BOUNDING_SPHERE;

typedef struct VERTEX
{
	short vx;
	short vy;
	short vz;
	
	short nx;
	short ny;
	short nz;
	
	short shade;
}
VERTEX;

typedef struct POLYGON
{
	char isTriangle;
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
	unsigned short v4;
	unsigned short textureInfo;
	unsigned char attributes;
	unsigned char unknown;	// garbage?
}
POLYGON;

typedef struct MESH
{
	BOUNDING_SPHERE boundingSphere;
	
	unsigned char usesNormals;
	unsigned short numVertices;
	VERTEX* vertices;
	
	unsigned short numPolygons;
	POLYGON* polygons;
}
MESH;

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

typedef struct MOVABLE
{
	unsigned int movableId;
	unsigned short numMeshes;
	unsigned int* meshIndices;
	unsigned int skeletonIndex;
	unsigned short numAnimations;
	ANIMATION* animations;
}
MOVABLE;

typedef struct STATIC
{
	unsigned int staticId;
	unsigned int meshIndex;
	short vx1;
	short vy1;
	short vz1;
	
	short vx2;
	short vy2;
	short vz2;
	
	short cx1;
	short cy1;
	short cz1;
	
	short cx2;
	short cy2;
	short cz2;
	
	unsigned short flags;
}
STATIC;

typedef struct WAD
{
	unsigned int version;
	
	// DONE
	unsigned int numTextureSamples;
	TEXTURE_SAMPLE* textureSamples;
	
	// DONE
	unsigned int numTexturePages;
	TEXTURE_PAGE* texturePages;
	
	// DONE
	/* In theory, movables can refer to the same skeleton. Let's check it in future.
	   If not, then move each skeleton instance to related mesh. */
	unsigned int numSkeletons;
	SKELETON* skeletons;
	
	// DONE
	unsigned int numMeshes;
	MESH* meshes;
	
	// DONE
	unsigned int numMovables;
	MOVABLE* movables;
	
	// DONE
	unsigned int numStatics;
	STATIC* statics;
}
WAD;

#endif /* wad_h */
