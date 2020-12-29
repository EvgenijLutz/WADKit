//
//  buffer_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef buffer_interface_h
#define buffer_interface_h

#include "wad_kit.h"

void bufferInitializeWithCapacity(WK_BUFFER* buffer, unsigned long capacity);
void bufferInitializeWithData(WK_BUFFER* buffer, const char* data, unsigned long length);
void bufferDeinitialize(WK_BUFFER* buffer);

unsigned long bufferGetEditorPosition(WK_BUFFER* buffer);
void bufferSetEditorPosition(WK_BUFFER* buffer, unsigned long position);
void bufferResetEditorPosition(WK_BUFFER* buffer);
void bufferClear(WK_BUFFER* buffer);

char* bufferRequestDataToRead(WK_BUFFER* buffer, unsigned long length, EXECUTE_RESULT* executeResult);

char bufferReadInt8(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
unsigned char bufferReadUInt8(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
short bufferReadInt16(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
unsigned short bufferReadUInt16(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
int bufferReadInt32(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
unsigned int bufferReadUInt32(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
long bufferReadInt64(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
unsigned long bufferReadUInt64(WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);

char* bufferRequestDataToWrite(WK_BUFFER* buffer, unsigned long length);

void bufferWriteData(WK_BUFFER* buffer, const char* data, unsigned long length);
void bufferWriteInt8(WK_BUFFER* buffer, char data);
void bufferWriteIntU8(WK_BUFFER* buffer, unsigned char data);
void bufferWriteInt16(WK_BUFFER* buffer, short data);
void bufferWriteIntU16(WK_BUFFER* buffer, unsigned short data);
void bufferWriteInt32(WK_BUFFER* buffer, int data);
void bufferWriteIntU32(WK_BUFFER* buffer, unsigned int data);
void bufferWriteInt64(WK_BUFFER* buffer, long data);
void bufferWriteIntU64(WK_BUFFER* buffer, unsigned long data);

#endif /* buffer_interface_h */
