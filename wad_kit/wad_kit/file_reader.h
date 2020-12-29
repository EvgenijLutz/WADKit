//
//  file_reader.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef file_reader_h
#define file_reader_h

#include "wad_kit.h"

typedef struct WK_FILE_READER
{
	WK_SYSTEM* system;
	
	WK_FILE_MODE mode;
	void* fileId;
	unsigned long size;
	
	/*!
	 Doesn't work properly. Try to avoid it.
	 */
	unsigned long position;
}
WK_FILE_READER;

#endif /* file_reader_h */
