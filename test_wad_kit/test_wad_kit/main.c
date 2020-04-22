//
//  main.c
//  test_wad_kit
//
//  Created by Евгений Лютц on 08.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "../../wad_kit/wad_kit/wad.h"
#include "../../wad_kit/wad_kit/wad_interface.h"
#include "../../wad_kit/wad_kit/execute_result.h"

//#define MEMORY_LEAKS_PROFILE_CRUTCH 1
#if defined(MEMORY_LEAKS_PROFILE_CRUTCH)
#include <unistd.h>
#endif

WAD* readWadFromFile(const char* wadPath, EXECUTE_RESULT* executeResult)
{
	FILE* file = fopen(wadPath, "rb");
	if (!file)
	{
		printf("Cannot read file\n");
		return NULL;
	}
	
	fseek(file, 0, SEEK_END);
	long dataSize = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char* data = malloc(dataSize);
	fread(data, dataSize, 1, file);
	fclose(file);
	
	WAD* wad = wadLoadFromWadData(data, dataSize, executeResult);
	if (!executeResult->succeeded)
	{
		printf("%s", executeResult->message);
		return NULL;
	}
	
	free(data);
	
	return wad;
}

int main(int argc, const char * argv[])
{
	/* * * Load WAD from file * * */
	const char* wadPath =
	//"/Users/eugene_lutz/Desktop/trle/wads/test/1-tutorial.wad";
	//"/Users/eugene_lutz/Desktop/trle/wads/1-tutorial/1-tutorial.wad";
	"/Users/eugene_lutz/Desktop/trle/wads/tut1/tut1.WAD";
	//"/Users/eugene_lutz/Desktop/trle/wads/tut1/export.WAD";
	EXECUTE_RESULT executeResult;
	WAD* wad = readWadFromFile(wadPath, &executeResult);
	if (!executeResult.succeeded)
	{
		printf("Cannot read file: %s", executeResult.message);
		return 1;
	}
	printf("File read successfully\n");
	
	
	/* Edit WAD */
	
	// First 13 movables will have the same skeleton
	for (unsigned int i = 0; i < 13; i++)
	{
		wad->movables[i].skeletonIndex = 0;
	}
	
	// Lara will hold crossobws instead of pistols
	/*for (unsigned int i = 0; i < wad->movables[1].numMeshes; i++)
	{
		wad->movables[1].meshIndices[i] = 11;
	}*/
	
	
	/* * * Save WAD to file * * */
	long dataSize = 0;
	unsigned char* exportData = wadGenerateWadData(wad, &dataSize, &executeResult);
	if (!executeResult.succeeded)
	{
		printf("Cannot export data: %s", executeResult.message);
		return 1;
	}
	const char* exportPath =
	//"/Users/eugene_lutz/Desktop/trle/wads/test/export.wad";
	"/Users/eugene_lutz/Desktop/trle/wads/tut1/export.WAD";
	//"/Users/eugene_lutz/Desktop/trle/wads/1-tutorial/export.WAD";
	FILE* exportFile = fopen(exportPath, "wb");
	if (exportFile)
	{
		fwrite(exportData, dataSize, 1, exportFile);
		fclose(exportFile);
		printf("File written successfully\n");
	}
	else
	{
		printf("Cannot create export file.\n");
	}
	wadRelease(wad);
	
	#if defined(MEMORY_LEAKS_PROFILE_CRUTCH)
	sleep(7);
	#endif
	
	return 0;
}
