//
//  system.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wk_system_h
#define wk_system_h

#include "wad_kit.h"

typedef struct WK_SYSTEM
{
	int createdDefault;
	WK_SYSTEM_GET_RESOURCE_FILE_PATH_FUNC getResourceFilePathFunc;
	
	WK_SYSTEM_OPEN_FILE_READER_FUNC openFileReaderFunc;
	WK_SYSTEM_CLOSE_FILE_READER_FUNC closeFileReaderFunc;
	WK_FILE_READER_GET_FILE_SIZE_FUNC fileReaderGetFileSizeFunc;
	WK_FILE_READER_GET_POSITION_FUNC fileReaderGetPositionFunc;
	WK_FILE_READER_READ_DATA_FUNC fileReaderReadDataFunc;
	WK_FILE_READER_READ_INT8_FUNC fileReaderReadInt8Func;
	WK_FILE_READER_READ_UINT8_FUNC fileReaderReadUInt8Func;
	WK_FILE_READER_READ_INT16_FUNC fileReaderReadInt16Func;
	WK_FILE_READER_READ_UINT16_FUNC fileReaderReadUInt16Func;
	WK_FILE_READER_READ_INT32_FUNC fileReaderReadInt32Func;
	WK_FILE_READER_READ_UINT32_FUNC fileReaderReadUInt32Func;
	
	void* userInfo;
}
WK_SYSTEM;

#endif /* wk_system_h */
