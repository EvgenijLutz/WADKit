//
//  wadCreateFromContentsOfResourceFile.c
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

#define WAD_TEXTURE_SAMPLE_SIZE (8)
#define WAD_ANIMATION_SIZE (40)

static void _wad_loadDataFromBuffer(WK_WAD* wad, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult)
{
	char errorMessage[2048];
	
	/// MARK:  SECTION 1 – VERSION
	const unsigned int version = bufferReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	if (version < 129 || version > 130)
	{
		sprintf(errorMessage, "Unsupported WAD file version: %d", version);
		executeResultSetFailed(executeResult, errorMessage);
		return;
	}
	wad->version = version;
	
	
	/// MARK: SECTION 2 – TEXTURES
	const unsigned int numTextureSamples = bufferReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// Too much for tiles in a WAD file... or?
	if (numTextureSamples > 8192)
	{
		sprintf(errorMessage, "Too much texture samples found in file: %d", numTextureSamples);
		executeResultSetFailed(executeResult, errorMessage);
		return;
	}
	const unsigned long textureSamplesTableSize = numTextureSamples * WAD_TEXTURE_SAMPLE_SIZE;
	const unsigned long textureTilesBufferEditorPosition = buffer->editorPosition;
	
	// Zuerst lesen wir die Texture-Seiten, danach die Samples
	bufferSetEditorPosition(buffer, buffer->editorPosition + textureSamplesTableSize);
	const unsigned int texturePagesBufferSize = bufferReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// Lesen Texture-Seiten
	const unsigned int numTexturePages = texturePagesBufferSize / WK_SIZE_OF_TEXTURE_PAGE;
	for (unsigned int i = 0; i < numTexturePages; i++)
	{
		TEXTURE_PAGE* texturePage = arrayAddItem(&wad->texturePages);
		texturePageInitializeFromBuffer(texturePage, wad, buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	const unsigned long meshesDataBufferEditorPosition = buffer->editorPosition;
	
	// Jetzt lesen wir die Samples
	bufferSetEditorPosition(buffer, textureTilesBufferEditorPosition);
	for (unsigned int i = 0; i < numTextureSamples; i++)
	{
		TEXTURE_SAMPLE* textureSample = arrayAddItem(&wad->textureSamples);
		textureSampleInitializeFromBuffer(textureSample, wad, buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	/// MARK: SECTION 3 – MESHES
	bufferSetEditorPosition(buffer, meshesDataBufferEditorPosition);
	const unsigned int numMeshPointers = bufferReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	if (numMeshPointers > 4096)
	{
		sprintf(errorMessage, "Too much mesh pointers found in file: %d", numMeshPointers);
		executeResultSetFailed(executeResult, errorMessage);
		return;
	}
	
	// Mesh pointers list, we will use it later
	const unsigned int meshPointersSize = numMeshPointers * 4;
	const unsigned int* raw_meshPointersList = (unsigned int*)bufferRequestDataToRead(buffer, meshPointersSize, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// Figuring out the end of meshes section, because it's not possible to deternime the number of meshes here
	const unsigned long meshDataPackageSize = bufferReadUInt32(buffer, executeResult) * 2;
	if (executeResultIsFailed(executeResult)) { return; }
	const unsigned long meshDataPackageEnd = buffer->editorPosition + meshDataPackageSize;
	
	// Make bizzare list of memory offsers for meshes because WAD wants so
	unsigned int* meshAddresses = malloc(sizeof(unsigned int) * numMeshPointers);
	const unsigned long meshDataPackageAddress = buffer->editorPosition;
	unsigned int numMeshes = 0;
	
	// Get each mesh data
	while (meshDataPackageEnd > buffer->editorPosition)
	{
		meshAddresses[numMeshes] = (unsigned int)(buffer->editorPosition - meshDataPackageAddress);
		
		MESH* mesh = arrayAddItem(&wad->meshes);
		meshInitializeFromBuffer(mesh, wad, buffer, executeResult);
		if (executeResultIsFailed(executeResult))
		{
			free(meshAddresses);
			return;
		}
		
		numMeshes++;
	}
	
	// MARK: SECTION 4 – ANIMATIONS
	
	/// In theory, we can share animations across objects. But we won't do this, because it will make headaches for everyone
	
	//
	
	free(meshAddresses);
}

WK_WAD* wadCreateFromContentsOfResourceFile(WK_SYSTEM* system, const char* name, EXECUTE_RESULT* executeResult)
{
	WK_STRING path;
	stringInitialize(&path, NULL);
	systemGetPathForResourceWithName(system, &path, name, "WAD", executeResult);
	if (executeResultIsFailed(executeResult)) {
		stringDeinitialize(&path);
		return NULL;
	}
	
	WK_FILE_READER* reader = systemOpenFileReader(system, path.data, WK_FILE_MODE_BINARY, executeResult);
	if (executeResultIsFailed(executeResult)) {
		stringDeinitialize(&path);
		return NULL;
	}
	stringDeinitialize(&path);
	
	WK_BUFFER buffer;
	bufferInitializeWithCapacity(&buffer, 512);
	fileReaderReadBuffer(reader, &buffer, fileReaderGetFileSize(reader), executeResult);
	if (executeResultIsFailed(executeResult)) {
		bufferDeinitialize(&buffer);
		fileReaderClose(reader, NULL);
		return NULL;
	}
	
	fileReaderClose(reader, executeResult);
	if (executeResultIsFailed(executeResult)) {
		bufferDeinitialize(&buffer);
		return NULL;
	}
	
	bufferResetEditorPosition(&buffer);
	WK_WAD* wad = wadCreate();
	_wad_loadDataFromBuffer(wad, &buffer, executeResult);
	bufferDeinitialize(&buffer);
	if (executeResultIsFailed(executeResult)) {
		wadRelease(wad);
		return NULL;
	}
	
	return wad;
}
