//
//  file_reader_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef file_reader_interface_h
#define file_reader_interface_h

#include "wad_kit.h"

WK_FILE_READER* systemOpenFileReader(WK_SYSTEM* system, const char* path, WK_FILE_MODE readMode, EXECUTE_RESULT* executeResult);
void fileReaderClose(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult);

unsigned long fileReaderGetFileSize(WK_FILE_READER* reader);

void fileReaderReadBuffer(WK_FILE_READER* reader, WK_BUFFER* outBuffer, unsigned long length, EXECUTE_RESULT* executeResult);

char fileReaderReadInt8(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult);
unsigned char fileReaderReadUInt8(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult);

short fileReaderReadInt16(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult);
unsigned short fileReaderReadUInt16(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult);

int fileReaderReadInt32(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult);
unsigned int fileReaderReadUInt32(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult);

#endif /* file_reader_interface_h */
