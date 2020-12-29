//
//  buffer.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef buffer_h
#define buffer_h

#include "wad_kit.h"

/*!
 Buffer editor.
 */
typedef struct WK_BUFFER
{
	unsigned long capacity;
	unsigned long length;
	unsigned long capacityIncrement;
	char* data;
	unsigned long editorPosition;
}
WK_BUFFER;

#endif /* buffer_h */
