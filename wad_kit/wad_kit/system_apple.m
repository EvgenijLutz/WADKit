//
//  io_apple.m
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#include "private_interface.h"

// MARK: - File paths

static void _systemApple_getResourceFilePath(WK_SYSTEM* system, WK_STRING* outPath, const char* name, const char* type, EXECUTE_RESULT* executeResult)
{
	NSString* resourceName = [NSString stringWithUTF8String:name];
	NSString* typeName = [NSString stringWithUTF8String:type];
	NSString* resourcePath = [NSBundle.mainBundle pathForResource:resourceName ofType:typeName];
	
	stringSetData(outPath, resourcePath.UTF8String);
	executeResultSetSucceeded(executeResult);
}


// MARK: - Semaphore

static void* _systemApple_createSemaphore(WK_SYSTEM* system, int value, void* userInfo)
{
	dispatch_semaphore_t semaphore = dispatch_semaphore_create(value);
	void* semaphoreId = (__bridge_retained void*)semaphore;
	return semaphoreId;
}

static void _systemApple_releaseSemaphore(WK_SYSTEM* system, void* semaphoreId, void* userInfo)
{
	dispatch_semaphore_t semaphore = (__bridge_transfer dispatch_semaphore_t)semaphoreId;
	semaphore = nil;
}

static void _systemApple_semaphoreEnter(WK_SYSTEM* system, void* semaphoreId, void* userInfo)
{
	dispatch_semaphore_t semaphore = (__bridge dispatch_semaphore_t)semaphoreId;
	dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
}

static void _systemApple_semaphoreLeave(WK_SYSTEM* system, void* semaphoreId, void* userInfo)
{
	dispatch_semaphore_t semaphore = (__bridge dispatch_semaphore_t)semaphoreId;
	dispatch_semaphore_signal(semaphore);
}


// MARK: - IO

static void* _systemApple_openReader(WK_SYSTEM* system, const char* path, WK_FILE_MODE readMode, EXECUTE_RESULT* executeResult)
{
	const char* mode = NULL;
	switch (readMode) {
		case WK_FILE_MODE_BINARY: mode = "rb"; break;
		default:
			executeResultSetFailed(executeResult, "Unsupported file mode.");
			return NULL;
	}
	
	FILE* file = fopen(path, mode);
	if (!file) {
		executeResultSetFailed(executeResult, "Cannot open file.");
		return NULL;
	}
	
	return file;
}

static void _systemApple_closeReader(WK_SYSTEM* system, void* fileId, EXECUTE_RESULT* executeResult)
{
	FILE* file = fileId;
	fclose(file);
	executeResultSetSucceeded(executeResult);
}

static unsigned int _systemApple_fileReaderGetFileSize(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult)
{
	FILE* file = fileId;
	
	long position = ftell(file);
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, position, SEEK_SET);
	
	executeResultSetSucceeded(executeResult);
	return (unsigned int)size;
}

static unsigned long _systemApple_fileReaderGetPosition(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult)
{
	FILE* file = fileId;
	long position = ftell(file);
	
	executeResultSetSucceeded(executeResult);
	return position;
}

static void _systemApple_fileReaderReadData(WK_FILE_READER* reader, char* outData, unsigned long length, void* fileId, EXECUTE_RESULT* executeResult)
{
	size_t read = fread(outData, length, 1, fileId);
	if (read != 1) {
		executeResultSetFailed(executeResult, "Cannot read data.");
		return;
	}
}

static char _systemApple_fileReaderReadInt8(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult) {
	int8_t value = 0;
	if (reader->mode == WK_FILE_MODE_BINARY) {
		size_t read = fread(&value, sizeof(int8_t), 1, fileId);
		if (read != 1) {
			executeResultSetFailed(executeResult, "Cannot read int8 value.");
			return 0;
		}
		reader->position = ftell(fileId);
	} else {
		char data[32];
		char* result = fgets(data, 32, fileId);
		if (result != data) {
			executeResultSetFailed(executeResult, "Cannot read int8 value.");
			return 0;
		}
		value = atoi(data);
		reader->position = ftell(fileId);
	}
	executeResultSetSucceeded(executeResult);
	return (int)value;
}

static unsigned char _systemApple_fileReaderReadUInt8(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult) {
	uint8_t value = 0;
	if (reader->mode == WK_FILE_MODE_BINARY) {
		size_t read = fread(&value, sizeof(uint8_t), 1, fileId);
		if (read != 1) {
			executeResultSetFailed(executeResult, "Cannot read uint8 value.");
			return 0;
		}
		reader->position = ftell(fileId);
	} else {
		char data[32];
		char* result = fgets(data, 32, fileId);
		if (result != data) {
			executeResultSetFailed(executeResult, "Cannot read uint8 value.");
			return 0;
		}
		value = atoi(data);
		reader->position = ftell(fileId);
	}
	executeResultSetSucceeded(executeResult);
	return (unsigned int)value;
}

static short _systemApple_fileReaderReadInt16(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult) {
	int16_t value = 0;
	if (reader->mode == WK_FILE_MODE_BINARY) {
		size_t read = fread(&value, sizeof(int16_t), 1, fileId);
		if (read != 1) {
			executeResultSetFailed(executeResult, "Cannot read int16 value.");
			return 0;
		}
		reader->position = ftell(fileId);
	} else {
		char data[32];
		char* result = fgets(data, 32, fileId);
		if (result != data) {
			executeResultSetFailed(executeResult, "Cannot read int16 value.");
			return 0;
		}
		value = atoi(data);
		reader->position = ftell(fileId);
	}
	executeResultSetSucceeded(executeResult);
	return (int)value;
}

static unsigned short _systemApple_fileReaderReadUInt16(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult) {
	uint16_t value = 0;
	if (reader->mode == WK_FILE_MODE_BINARY) {
		size_t read = fread(&value, sizeof(uint16_t), 1, fileId);
		if (read != 1) {
			executeResultSetFailed(executeResult, "Cannot read uint16 value.");
			return 0;
		}
		reader->position = ftell(fileId);
	} else {
		char data[32];
		char* result = fgets(data, 32, fileId);
		if (result != data) {
			executeResultSetFailed(executeResult, "Cannot read uint16 value.");
			return 0;
		}
		value = atoi(data);
		reader->position = ftell(fileId);
	}
	executeResultSetSucceeded(executeResult);
	return (unsigned int)value;
}

static int _systemApple_fileReaderReadInt32(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult) {
	int32_t value = 0;
	if (reader->mode == WK_FILE_MODE_BINARY) {
		size_t read = fread(&value, sizeof(int32_t), 1, fileId);
		if (read != 1) {
			executeResultSetFailed(executeResult, "Cannot read int32 value.");
			return 0;
		}
		reader->position = ftell(fileId);
	} else {
		char data[32];
		char* result = fgets(data, 32, fileId);
		if (result != data) {
			executeResultSetFailed(executeResult, "Cannot read int32 value.");
			return 0;
		}
		value = atoi(data);
		reader->position = ftell(fileId);
	}
	executeResultSetSucceeded(executeResult);
	return (int)value;
}

static unsigned int _systemApple_fileReaderReadUInt32(WK_FILE_READER* reader, void* fileId, EXECUTE_RESULT* executeResult) {
	uint32_t value = 0;
	if (reader->mode == WK_FILE_MODE_BINARY) {
		size_t read = fread(&value, sizeof(uint32_t), 1, fileId);
		if (read != 1) {
			executeResultSetFailed(executeResult, "Cannot read uint32 value.");
			return 0;
		}
		reader->position = ftell(fileId);
	} else {
		char data[32];
		char* result = fgets(data, 32, fileId);
		if (result != data) {
			executeResultSetFailed(executeResult, "Cannot read uint32 value.");
			return 0;
		}
		value = atoi(data);
		reader->position = ftell(fileId);
	}
	executeResultSetSucceeded(executeResult);
	return (unsigned int)value;
}


// MARK: - Timing

static double _systemApple_getCurrentTime()
{
	return (double)CACurrentMediaTime();
}


// MARK: - Threading


// MARK: - System implementation for Apple platforms

WK_SYSTEM* systemCreateDefaultApple(void)
{
	WK_SYSTEM* system = malloc(sizeof(WK_SYSTEM));
	system->createdDefault = 1;
	system->getResourceFilePathFunc = _systemApple_getResourceFilePath;
	
	system->createSemaphoreFunc = _systemApple_createSemaphore;
	system->releaseSemaphoreFunc = _systemApple_releaseSemaphore;
	system->semaphoreEnterFunc = _systemApple_semaphoreEnter;
	system->semaphoreLeaveFunc = _systemApple_semaphoreLeave;
	
	system->openFileReaderFunc = _systemApple_openReader;
	system->closeFileReaderFunc = _systemApple_closeReader;
	system->fileReaderGetFileSizeFunc = _systemApple_fileReaderGetFileSize;
	system->fileReaderGetPositionFunc = _systemApple_fileReaderGetPosition;
	system->fileReaderReadDataFunc = _systemApple_fileReaderReadData;
	system->fileReaderReadInt8Func = _systemApple_fileReaderReadInt8;
	system->fileReaderReadUInt8Func = _systemApple_fileReaderReadUInt8;
	system->fileReaderReadInt16Func = _systemApple_fileReaderReadInt16;
	system->fileReaderReadUInt16Func = _systemApple_fileReaderReadUInt16;
	system->fileReaderReadInt32Func = _systemApple_fileReaderReadInt32;
	system->fileReaderReadUInt32Func = _systemApple_fileReaderReadUInt32;
	
	system->getCurrentTimeFunc = _systemApple_getCurrentTime;
	
	return system;
}

void systemReleaseDefaultApple(WK_SYSTEM* system)
{
	free(system);
}
