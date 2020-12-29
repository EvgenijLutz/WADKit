//
//  string_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

void stringInitialize(WK_STRING* string, const char* data)
{
	assert(string);
	
	if (data)
	{
		string->length = (unsigned int)strlen(data);
		string->capacity = string->length + 1;
		string->data = malloc(string->capacity);
		memcpy(string->data, data, string->capacity);
	}
	else
	{
		string->capacity = 1;
		string->length = 0;
		string->data = malloc(sizeof(char) * 1);
		string->data[0] = 0;
	}
}

void stringDeinitialize(WK_STRING* string)
{
	assert(string);
	free(string->data);
#if DEBUG
	memset(string, 0, sizeof(WK_STRING));
#endif
}


char* stringGetData(WK_STRING* string)
{
	assert(string);
	return string->data;
}

void stringSetData(WK_STRING* string, const char* data)
{
	assert(string);
	
	if (!data)
	{
		string->length = 0;
		string->data[0] = 0;
		return;
	}
	
	unsigned int length = (unsigned int)strlen(data);
	if (length < string->capacity)
	{
		memcpy(string->data, data, length + 1);
	}
	else
	{
		string->capacity = length + 1;
		string->length = length;
		
		string->data = realloc(string->data, sizeof(char) * string->capacity);
		assert(string->data);
		
		memcpy(string->data, data, string->capacity);
	}
}

void stringSetString(WK_STRING* string, WK_STRING* source)
{
	assert(string);
	assert(source);
	stringSetData(string, source->data);
}

