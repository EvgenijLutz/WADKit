//
//  string.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef string_h
#define string_h

#include "wad_kit.h"

/*!
 @abstract Structure representing string data.
 @discussion Don't judge me.
 */
typedef struct WK_STRING
{
	unsigned int capacity;
	unsigned int length;
	char* data;
}
WK_STRING;

#endif /* string_h */
