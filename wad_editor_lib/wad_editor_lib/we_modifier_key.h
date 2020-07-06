//
//  we_modifier_key.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef we_modifier_key_h
#define we_modifier_key_h

typedef enum WE_MODIFIER_KEY
{
	WE_MODIFIER_KEY_SHIFT = 0,
	WE_MODIFIER_KEY_CONTROL,
	WE_MODIFIER_KEY_OPTION,
	WE_MODIFIER_KEY_COMMAND,
	WE_MODIFIER_KEY_FUNCTION,
	WE_MODIFIER_KEY_UNKNOWN,
	WE_MODIFIER_KEY_NUM_KEYS
}
WE_MODIFIER_KEY;

#endif /* we_modifier_key_h */
