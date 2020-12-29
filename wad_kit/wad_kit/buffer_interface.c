//
//  buffer_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

static void _buffer_requestMoreCapacity(WK_BUFFER* buffer, unsigned long desiredCapacity)
{
#if DEBUG
	unsigned long oldCapacity = buffer->capacity;
#endif
	
	unsigned long capacity = 0;
	if (buffer->capacity + buffer->capacityIncrement > desiredCapacity)
	{
		const double tmp = ceil((double)(buffer->capacity + buffer->capacityIncrement) / (double)buffer->capacityIncrement) * (double)buffer->capacityIncrement;
		capacity = (unsigned long)tmp;
	}
	else
	{
		const double tmp = ceil((double)desiredCapacity / (double)buffer->capacityIncrement) * (double)buffer->capacityIncrement;
		capacity = (unsigned long)tmp;
	}
	
	// Limit allocation of 50Mb
	assert(capacity < (50 * 1024 * 1024));
	
	buffer->capacity = capacity;
	buffer->data = realloc(buffer->data, capacity);
	assert(buffer->data);
	
#if DEBUG
	memset(&(buffer->data[oldCapacity]), 0, buffer->capacity - oldCapacity);
#endif
}

void bufferInitializeWithCapacity(WK_BUFFER* buffer, unsigned long capacity)
{
	assert(buffer);
	assert(capacity > 0);
	
	buffer->capacity = capacity;
	buffer->length = 0;
	buffer->capacityIncrement = capacity > 256 ? capacity : 256;
	buffer->data = malloc(capacity);
#if DEBUG
	memset(buffer->data, 0, buffer->capacity);
#endif
	buffer->editorPosition = 0;
}

void bufferInitializeWithData(WK_BUFFER* buffer, const char* data, unsigned long length)
{
	assert(buffer);
	assert(data);
	assert(length > 0);
	
	bufferInitializeWithCapacity(buffer, length);
	memcpy(buffer->data, data, length);
}

void bufferDeinitialize(WK_BUFFER* buffer)
{
	assert(buffer);
	
	free(buffer->data);
#if DEBUG
	memset(buffer, 0, sizeof(WK_BUFFER));
#endif
}


unsigned long bufferGetEditorPosition(WK_BUFFER* buffer)
{
	assert(buffer);
	return buffer->editorPosition;
}

void bufferSetEditorPosition(WK_BUFFER* buffer, unsigned long position)
{
	assert(buffer);
	assert(buffer->length >= position);
	buffer->editorPosition = position;
}

void bufferResetEditorPosition(WK_BUFFER* buffer)
{
	assert(buffer);
	buffer->editorPosition = 0;
}

void bufferClear(WK_BUFFER* buffer)
{
	assert(buffer);
	bufferResetEditorPosition(buffer);
	buffer->length = 0;
#if DEBUG
	memset(buffer->data, 0, buffer->capacity);
#endif
}


// Is this a dream
// I feel the earth beneath my feet

char* bufferRequestDataToRead(WK_BUFFER* buffer, unsigned long length, EXECUTE_RESULT* executeResult)
{
	assert(buffer);
	if (buffer->editorPosition + length > buffer->capacity) {
		char message[1024];
		sprintf(message, "Requested too much data to retrieve. %ld bytes were requested when %ld bytes were available (buffer capacity: %ld, editor position: %ld).", length, buffer->capacity - buffer->editorPosition, buffer->capacity, buffer->editorPosition);
		executeResultSetFailed(executeResult, message);
		return NULL;
	}
	
	char* data = &(buffer->data[buffer->editorPosition]);
	buffer->editorPosition += length;
	return data;
}

static void _buffer_readRawData(WK_BUFFER* buffer, char* data, unsigned long length, EXECUTE_RESULT* executeResult)
{
	assert(buffer);
	assert(data);
	assert(length > 0);
	
	if (buffer->editorPosition == buffer->capacity) {
		executeResultSetFailed(executeResult, "The end of buffer is reached.");
		return;
	}
	else if (buffer->editorPosition + length > buffer->capacity) {
		char message[1024];
		sprintf(message, "Requested too much data to retrieve. %ld bytes were requested when %ld bytes were available (buffer capacity: %ld, editor position: %ld).", length, buffer->capacity - buffer->editorPosition, buffer->capacity, buffer->editorPosition);
		executeResultSetFailed(executeResult, message);
		return;
	}
	
	memcpy(data, &(buffer->data[buffer->editorPosition]), length);
	buffer->editorPosition += length;
	
	executeResultSetSucceeded(executeResult);
}

char bufferReadInt8(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult) {
	char data = 0;
	_buffer_readRawData(buffer, (char*)&data, sizeof(int8_t), executeResult);
	return data;
}

unsigned char bufferReadUInt8(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult) {
	unsigned char data = 0;
	_buffer_readRawData(buffer, (char*)&data, sizeof(uint8_t), executeResult);
	return data;
}

short bufferReadInt16(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult) {
	short data = 0;
	_buffer_readRawData(buffer, (char*)&data, sizeof(int16_t), executeResult);
	return data;
}

unsigned short bufferReadUInt16(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult) {
	unsigned short data = 0;
	_buffer_readRawData(buffer, (char*)&data, sizeof(uint16_t), executeResult);
	return data;
}

int bufferReadInt32(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult) {
	int data = 0;
	_buffer_readRawData(buffer, (char*)&data, sizeof(int32_t), executeResult);
	return data;
}

unsigned int bufferReadUInt32(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult) {
	unsigned int data = 0;
	_buffer_readRawData(buffer, (char*)&data, sizeof(uint32_t), executeResult);
	return data;
}

long bufferReadInt64(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult) {
	long data = 0;
	_buffer_readRawData(buffer, (char*)&data, sizeof(int64_t), executeResult);
	return data;
}

unsigned long bufferReadUInt64(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult) {
	unsigned long data = 0;
	_buffer_readRawData(buffer, (char*)&data, sizeof(uint64_t), executeResult);
	return data;
}


char* bufferRequestDataToWrite(WK_BUFFER* buffer, unsigned long length)
{
	assert(buffer);
	
	unsigned long requestedCapacity = buffer->editorPosition + length;
	if (buffer->capacity < requestedCapacity) {
		_buffer_requestMoreCapacity(buffer, requestedCapacity);
	}
	
	char* data = &(buffer->data[buffer->editorPosition]);
#if DEBUG
	memset(data, 0, length);
#endif
	
	buffer->length += length;
	buffer->editorPosition += length;
	
	return data;
}


void bufferWriteData(WK_BUFFER* buffer, const char* data, unsigned long length)
{
	assert(buffer);
	
	unsigned long requestedCapacity = buffer->editorPosition + length;
	if (buffer->capacity < requestedCapacity) {
		_buffer_requestMoreCapacity(buffer, requestedCapacity);
	}
	
	memcpy(&(buffer->data[buffer->editorPosition]), data, length);
	buffer->length += length;
	buffer->editorPosition += length;
}

void bufferWriteInt8(WK_BUFFER* buffer, char data) {
	bufferWriteData(buffer, (char*)&data, sizeof(int8_t));
}

void bufferWriteIntU8(WK_BUFFER* buffer, unsigned char data) {
	bufferWriteData(buffer, (char*)&data, sizeof(uint8_t));
}

void bufferWriteInt16(WK_BUFFER* buffer, short data) {
	bufferWriteData(buffer, (char*)&data, sizeof(int16_t));
}

void bufferWriteIntU16(WK_BUFFER* buffer, unsigned short data) {
	bufferWriteData(buffer, (char*)&data, sizeof(uint16_t));
}

void bufferWriteInt32(WK_BUFFER* buffer, int data) {
	bufferWriteData(buffer, (char*)&data, sizeof(int32_t));
}

void bufferWriteIntU32(WK_BUFFER* buffer, unsigned int data) {
	bufferWriteData(buffer, (char*)&data, sizeof(uint32_t));
}

void bufferWriteInt64(WK_BUFFER* buffer, long data) {
	bufferWriteData(buffer, (char*)&data, sizeof(int64_t));
}

void bufferWriteIntU64(WK_BUFFER* buffer, unsigned long data) {
	bufferWriteData(buffer, (char*)&data, sizeof(uint64_t));
}
