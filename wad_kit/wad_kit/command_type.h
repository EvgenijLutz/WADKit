//
//  command_type.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef command_type_h
#define command_type_h

#include "wad_kit.h"

typedef enum WK_COMMAND_TYPE
{
	WK_COMMAND_TYPE_TRANSLATE = 1,
	WK_COMMAND_TYPE_JUMP = 2,
	WK_COMMAND_TYPE_GUN = 3,
	WK_COMMAND_TYPE_DEATH = 4,
	WK_COMMAND_TYPE_PLAY_SOUND = 5,
	WK_COMMAND_TYPE_ACTION = 6
}
WK_COMMAND_TYPE;

#endif /* command_type_h */
