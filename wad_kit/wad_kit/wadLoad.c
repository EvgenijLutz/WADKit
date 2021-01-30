//
//  wadLoad.c
//  wad_kit
//
//  Created by Евгений Лютц on 28.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

#define WAD_TEXTURE_SAMPLE_SIZE (8)
#define WAD_ANIMATION_SIZE (40)
#define WAD_STATE_CHANGE_SIZE (6)
#define WAD_DISPATCH_SIZE (8)
#define WAD_MOVABLE_SIZE (18)
#define WAD_STATIC_SIZE (32)

static void _wad_loadDataFromLoader(WK_WAD_LOAD_INFO* loadInfo)
{
	_Static_assert(sizeof(RAW_KEYFRAME) == 18, "Size of RAW_KEYFRAME has to be 18");
	_Static_assert(sizeof(RAW_ANIMATION) == WAD_ANIMATION_SIZE, "Size of RAW_ANIMATION has to be 40");
	_Static_assert(sizeof(RAW_STATE_CHANGE) == WAD_STATE_CHANGE_SIZE, "Size of RAW_STATE_CHANGE has to be 6");
	_Static_assert(sizeof(RAW_DISPATCH) == WAD_DISPATCH_SIZE, "Size of RAW_DISPATCH has to be 8");
	_Static_assert(sizeof(RAW_MOVABLE) == WAD_MOVABLE_SIZE, "Size of RAW_MOVABLE has to be 18");
	_Static_assert(sizeof(RAW_STATIC) == WAD_STATIC_SIZE, "Size of RAW_STATIC has to be 32");
	
	char errorMessage[2048];
	WK_WAD* wad = loadInfo->wad;
	BUFFER_READER* buffer = loadInfo->buffer;
	EXECUTE_RESULT* executeResult = loadInfo->executeResult;
	
	/// MARK:  SECTION 1 – VERSION
	const unsigned int version = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	if (version < 129 || version > 130)
	{
		sprintf(errorMessage, "Unsupported WAD file version: %d", version);
		executeResultSetFailed(executeResult, errorMessage);
		return;
	}
	wad->version = version;
	
	
	/// MARK: SECTION 2 – TEXTURES
	const unsigned int numTextureSamples = bufferReaderReadUInt32(buffer, executeResult);
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
	bufferReaderSetEditorPosition(buffer, buffer->editorPosition + textureSamplesTableSize);
	const unsigned int texturePagesBufferSize = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// Lesen Texture-Seiten
	const unsigned int numTexturePages = texturePagesBufferSize / WK_SIZE_OF_TEXTURE_PAGE;
	for (unsigned int i = 0; i < numTexturePages; i++)
	{
		WK_TEXTURE_PAGE* texturePage = magicArrayAddItem(&wad->texturePages);
		texturePageInitializeFromBuffer(texturePage, wad, buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	const unsigned long meshesDataBufferEditorPosition = buffer->editorPosition;
	
	// Jetzt lesen wir die Samples
	bufferReaderSetEditorPosition(buffer, textureTilesBufferEditorPosition);
	for (unsigned int i = 0; i < numTextureSamples; i++)
	{
		WK_TEXTURE_SAMPLE* textureSample = magicArrayAddItem(&wad->textureSamples);
		textureSampleInitializeFromBuffer(textureSample, wad, buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
	}
	
	/// MARK: SECTION 3 – MESHES
	bufferReaderSetEditorPosition(buffer, meshesDataBufferEditorPosition);
	loadInfo->numMeshPointers = bufferReaderReadUInt32(buffer, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	if (loadInfo->numMeshPointers > 4096)
	{
		sprintf(errorMessage, "Too much mesh pointers found in file: %d", loadInfo->numMeshPointers);
		executeResultSetFailed(executeResult, errorMessage);
		return;
	}
	
	// Mesh pointers list, we will use it later
	loadInfo->meshPointersDataSize = loadInfo->numMeshPointers * 4;
	loadInfo->meshPointersDataLocation = buffer->editorPosition;
	loadInfo->meshPointers = (unsigned int*)bufferReaderRequestDataToRead(buffer, loadInfo->meshPointersDataSize, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// Figuring out the end of meshes section, because it's not possible to deternime the number of meshes here
	const unsigned long meshDataSize = bufferReaderReadUInt32(buffer, executeResult) * 2;
	if (executeResultIsFailed(executeResult)) { return; }
	const unsigned long meshDataPackageEnd = buffer->editorPosition + meshDataSize;
	
	// Make bizzare list of memory offsers for meshes because WAD wants so
	loadInfo->numMeshDataOffsets = loadInfo->numMeshPointers;
	loadInfo->meshDataOffsets = malloc(sizeof(unsigned int) * loadInfo->numMeshDataOffsets);
	const unsigned long meshDataLocation = buffer->editorPosition;
	unsigned int numMeshes = 0;
	
	// Get each mesh data
	while (meshDataPackageEnd > buffer->editorPosition)
	{
		loadInfo->meshDataOffsets[numMeshes] = (unsigned int)(buffer->editorPosition - meshDataLocation);
		
		MESH* mesh = magicArrayAddItem(&wad->meshes);
		meshInitializeFromBuffer(mesh, wad, buffer, executeResult);
		if (executeResultIsFailed(executeResult)) { return; }
		
		numMeshes++;
	}
	
	// MARK: SECTION 4 – ANIMATIONS
	
	/// In theory, we can share animations across objects. But we won't do this, because it will make headaches for everyone.
	
	// Animations
	loadInfo->numAnimations = bufferReaderReadUInt32(buffer, executeResult);				// 731
	if (executeResultIsFailed(executeResult)) { return; }
	loadInfo->animationsDataSize = WAD_ANIMATION_SIZE * loadInfo->numAnimations;
	loadInfo->animationsDataLocation = buffer->editorPosition;
	loadInfo->rawAnimations = (RAW_ANIMATION*)bufferReaderRequestDataToRead(buffer, loadInfo->animationsDataSize, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// State changes
	const unsigned int numStateChanges = bufferReaderReadUInt32(buffer, executeResult);	// 487
	if (executeResultIsFailed(executeResult)) { return; }
	loadInfo->stateChangesDataSize = WAD_STATE_CHANGE_SIZE * numStateChanges;
	loadInfo->stateChangesDataLocation = buffer->editorPosition;
	loadInfo->rawStateChanges = (RAW_STATE_CHANGE*)bufferReaderRequestDataToRead(buffer, loadInfo->stateChangesDataSize, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// Dispatches
	const unsigned int numDispatches = bufferReaderReadUInt32(buffer, executeResult);		// 574
	if (executeResultIsFailed(executeResult)) { return; }
	loadInfo->dispatchesDataSize = WAD_DISPATCH_SIZE * numDispatches;
	loadInfo->dispatchesDataLocation = buffer->editorPosition;
	loadInfo->rawDispatches = (RAW_DISPATCH*)bufferReaderRequestDataToRead(buffer, loadInfo->dispatchesDataSize, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// Commands
	const unsigned int numCommandWords = bufferReaderReadUInt32(buffer, executeResult);	// 2411
	if (executeResultIsFailed(executeResult)) { return; }
	loadInfo->commandsDataSize = numCommandWords * 2;
	loadInfo->commandsDataLocation = buffer->editorPosition;
	bufferReaderSetEditorPosition(buffer, loadInfo->commandsDataLocation + loadInfo->commandsDataSize);
	
	// Links
	const unsigned int numLinksDWords = bufferReaderReadUInt32(buffer, executeResult);	// 1544
	if (executeResultIsFailed(executeResult)) { return; }
	loadInfo->linksDataSize = numLinksDWords * 4;
	loadInfo->linksDataLocation = buffer->editorPosition;
	bufferReaderSetEditorPosition(buffer, loadInfo->linksDataLocation + loadInfo->linksDataSize);
	
	// Keyframes
	loadInfo->numKeyframesWords = bufferReaderReadUInt32(buffer, executeResult);	// 239668
	if (executeResultIsFailed(executeResult)) { return; }
	loadInfo->keyframesDataSize = loadInfo->numKeyframesWords * 2;
	loadInfo->keyframesDataLocation = buffer->editorPosition;
	bufferReaderSetEditorPosition(buffer, loadInfo->keyframesDataLocation + loadInfo->keyframesDataSize);
	
	/// MARK: SECTION 5 – MODELS
	
	// Movables
	loadInfo->numMovables = bufferReaderReadUInt32(buffer, executeResult);				// 109
	if (executeResultIsFailed(executeResult)) { return; }
	loadInfo->movablesDataSize = WAD_MOVABLE_SIZE * loadInfo->numMovables;
	loadInfo->movablesDataLocation = buffer->editorPosition;
	loadInfo->rawMovables = (RAW_MOVABLE*)bufferReaderRequestDataToRead(buffer, loadInfo->movablesDataSize, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	// Statics
	loadInfo->numStatics = bufferReaderReadUInt32(buffer, executeResult);					// 11
	if (executeResultIsFailed(executeResult)) { return; }
	loadInfo->staticsDataSize = WAD_STATIC_SIZE * loadInfo->numStatics;
	loadInfo->staticsDataLocation = buffer->editorPosition;
	loadInfo->rawStatics = (RAW_STATIC*)bufferReaderRequestDataToRead(buffer, loadInfo->staticsDataSize, executeResult);
	if (executeResultIsFailed(executeResult)) { return; }
	
	/// MARK: Process received data
	
	// Movables
	bufferReaderSetEditorPosition(buffer, loadInfo->movablesDataLocation);
	for (unsigned int i = 0; i < loadInfo->numMovables; i++)
	{
		RAW_MOVABLE* rawMovable = &loadInfo->rawMovables[i];
		MOVABLE* movable = magicArrayAddItem(&wad->movables);
		movableInitialize(movable, rawMovable, loadInfo);
	}
	
	// Statics
	for (unsigned int i = 0; i < loadInfo->numStatics; i++)
	{
		RAW_STATIC* rawStatic = &loadInfo->rawStatics[i];
		STATIC* movable = magicArrayAddItem(&wad->statics);
		staticInitialize(movable, rawStatic, loadInfo);
	}
}

WK_WAD* wadCreateFromContentsOfBuffer(BUFFER* buffer, EXECUTE_RESULT* executeResult)
{
	assert(buffer);
	
	WK_WAD* wad = wadCreate();
	
	WK_WAD_LOAD_INFO loadInfo;
	memset(&loadInfo, 0, sizeof(WK_WAD_LOAD_INFO));
	loadInfo.wad = wad;
	loadInfo.buffer = bufferReaderCreate(buffer);
	loadInfo.executeResult = executeResult;
	
	_wad_loadDataFromLoader(&loadInfo);

	if (loadInfo.meshDataOffsets)
	{
		free(loadInfo.meshDataOffsets);
	}
	bufferReaderRelease(loadInfo.buffer);
	
	return wad;
}

WK_WAD* wadCreateFromContentsOfResourceFile(SYSTEM* system, const char* name, EXECUTE_RESULT* executeResult)
{
	STRING path;
	stringInitializeWithUTF8String(&path, NULL);
	systemGetPathForResourceWithName(system, &path, name, "WAD", executeResult);
	if (executeResultIsFailed(executeResult)) {
		systemGetPathForResourceWithName(system, &path, name, "wad", executeResult);
	}
	if (executeResultIsFailed(executeResult)) {
		stringDeinitialize(&path);
		return NULL;
	}
	
	FILE_READER* reader = systemOpenFileReader(system, path.data, FILE_MODE_BINARY, executeResult);
	if (executeResultIsFailed(executeResult)) {
		stringDeinitialize(&path);
		return NULL;
	}
	stringDeinitialize(&path);
	
	BUFFER* buffer = bufferCreate(512);
	fileReaderReadToBuffer(reader, buffer, fileReaderGetFileSize(reader), executeResult);
	if (executeResultIsFailed(executeResult)) {
		bufferRelease(buffer);
		fileReaderClose(reader, NULL);
		return NULL;
	}
	
	fileReaderClose(reader, executeResult);
	if (executeResultIsFailed(executeResult)) {
		bufferRelease(buffer);
		return NULL;
	}
	
	WK_WAD* wad = wadCreateFromContentsOfBuffer(buffer, executeResult);
	bufferRelease(buffer);
	if (executeResultIsFailed(executeResult)) {
		wadRelease(wad);
		return NULL;
	}
	
	return wad;
}
