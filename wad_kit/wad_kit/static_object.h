//
//  static_object.h
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef static_object_h
#define static_object_h

typedef enum STATIC_ID
{
	STATIC_ID_PLANT0	= 0,
	STATIC_ID_PLANT1	= 1,
	STATIC_ID_PLANT2	= 2,
	STATIC_ID_PLANT3	= 3,
	STATIC_ID_PLANT4	= 4,
	STATIC_ID_PLANT5	= 5,
	STATIC_ID_PLANT6	= 6,
	STATIC_ID_PLANT7	= 7,
	STATIC_ID_PLANT8	= 8,
	STATIC_ID_PLANT9	= 9
}
STATIC_ID;

typedef struct STATIC
{
	unsigned int staticId;	// TODO: STATIC_ID?
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

#endif /* static_object_h */
