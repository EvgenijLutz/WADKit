//
//  file_reader_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

WK_FILE_READER* systemOpenFileReader(WK_SYSTEM* system, const char* path, WK_FILE_MODE readMode, EXECUTE_RESULT* executeResult)
{
	assert(system);
	
	void* fileId = system->openFileReaderFunc(system, path, readMode, executeResult);
	if (executeResultIsFailed(executeResult)) {
		return NULL;
	}
	
	WK_FILE_READER* reader = malloc(sizeof(WK_FILE_READER));
	reader->system = system;
	reader->mode = readMode;
	reader->fileId = fileId;
	reader->size = 0;
	reader->position = 0;
	
	reader->size = system->fileReaderGetFileSizeFunc(reader, fileId, executeResult);
	if (executeResultIsFailed(executeResult)) {
		fileReaderClose(reader, NULL);
		free(reader);
		return NULL;
	}
	
	return reader;
}

void fileReaderClose(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult)
{
	assert(reader);
	assert(reader->fileId);
	
	reader->system->closeFileReaderFunc(reader->system, reader->fileId, executeResult);
	if (executeResultIsFailed(executeResult)) {
		return;
	}
#if DEBUG
	memset(reader, 0, sizeof(WK_FILE_READER));
#endif
	
	free(reader);
	executeResultSetSucceeded(executeResult);
}


unsigned long fileReaderGetFileSize(WK_FILE_READER* reader)
{
	assert(reader);
	return reader->size;
}


void fileReaderReadBuffer(WK_FILE_READER* reader, WK_BUFFER* outBuffer, unsigned long length, EXECUTE_RESULT* executeResult)
{
	assert(reader);
	assert(outBuffer);
	
	char* data = bufferRequestDataToWrite(outBuffer, length);
	reader->system->fileReaderReadDataFunc(reader, data, length, reader->fileId, executeResult);
	if (executeResultIsFailed(executeResult)) {
		return;
	}
	
	reader->system->fileReaderGetPositionFunc(reader, reader->fileId, executeResult);
}


char fileReaderReadInt8(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult) {
	assert(reader);
	return reader->system->fileReaderReadInt8Func(reader, reader->fileId, executeResult);
}

unsigned char fileReaderReadUInt8(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult) {
	assert(reader);
	return reader->system->fileReaderReadUInt8Func(reader, reader->fileId, executeResult);
}


short fileReaderReadInt16(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult) {
	assert(reader);
	return reader->system->fileReaderReadInt16Func(reader, reader->fileId, executeResult);
}

unsigned short fileReaderReadUInt16(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult) {
	assert(reader);
	return reader->system->fileReaderReadUInt16Func(reader, reader->fileId, executeResult);
}


int fileReaderReadInt32(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult) {
	assert(reader);
	return reader->system->fileReaderReadInt32Func(reader, reader->fileId, executeResult);
}

unsigned int fileReaderReadUInt32(WK_FILE_READER* reader, EXECUTE_RESULT* executeResult) {
	assert(reader);
	return reader->system->fileReaderReadUInt32Func(reader, reader->fileId, executeResult);
}
