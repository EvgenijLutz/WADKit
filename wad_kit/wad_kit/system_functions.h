//
//  system_functions.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef system_functions_h
#define system_functions_h

#include "wad_kit.h"

typedef void (* WK_SYSTEM_GET_RESOURCE_FILE_PATH_FUNC)(WK_SYSTEM* system, WK_STRING* outPath, const char* name, const char* type, EXECUTE_RESULT* executeResult);

typedef void* (* WK_SYSTEM_OPEN_FILE_READER_FUNC)(WK_SYSTEM* system, const char* path, WK_FILE_MODE readMode, EXECUTE_RESULT* executeResult);
typedef void (* WK_SYSTEM_CLOSE_FILE_READER_FUNC)(WK_SYSTEM* system, void* fileId, EXECUTE_RESULT* executeResult);
typedef unsigned int (* WK_FILE_READER_GET_FILE_SIZE_FUNC)(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult);
typedef unsigned long (* WK_FILE_READER_GET_POSITION_FUNC)(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult);
typedef void (* WK_FILE_READER_READ_DATA_FUNC)(WK_FILE_READER* reader, char* outData, unsigned long size, void* fileId, EXECUTE_RESULT* executeResult);
typedef char (* WK_FILE_READER_READ_INT8_FUNC)(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult);
typedef unsigned char (* WK_FILE_READER_READ_UINT8_FUNC)(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult);
typedef short (* WK_FILE_READER_READ_INT16_FUNC)(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult);
typedef unsigned short (* WK_FILE_READER_READ_UINT16_FUNC)(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult);
typedef int (* WK_FILE_READER_READ_INT32_FUNC)(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult);
typedef unsigned int (* WK_FILE_READER_READ_UINT32_FUNC)(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult);

#endif /* system_functions_h */
