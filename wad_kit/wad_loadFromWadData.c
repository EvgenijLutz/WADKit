//
//  wad_loadFromWadData.c
//  wad_kit
//
//  Created by Евгений Лютц on 15.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "wad_interface.h"
#include "wad.h"
#include "raw_wad.h"
#include "execute_result.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROTATION_USED_THREE_AXES	0x0000
#define ROTATION_USED_X_AXIS		0x4000
#define ROTATION_USED_Y_AXIS		0x8000
#define ROTATION_USED_Z_AXIS		0xc000

typedef struct DATA_READER
{
	const unsigned char* data;
	long size;
	long position;
}
DATA_READER;

static unsigned int readUInt(DATA_READER* reader)
{
	unsigned int value = *(const unsigned int*)(reader->data + reader->position);
	reader->position += sizeof(unsigned int);
	return value;
}

static int readInt(DATA_READER* reader)
{
	int value = *(const int*)(reader->data + reader->position);
	reader->position += sizeof(int);
	return value;
}

static long readLong(DATA_READER* reader)
{
	long value = *(const long*)(reader->data + reader->position);
	reader->position += 8;//sizeof(long);
	return value;
}

static unsigned char readUChar(DATA_READER* reader)
{
	unsigned char value = *(const unsigned char*)(reader->data + reader->position);
	reader->position += sizeof(unsigned char);
	return value;
}

static char readChar(DATA_READER* reader)
{
	char value = *(const char*)(reader->data + reader->position);
	reader->position += sizeof(char);
	return value;
}

static unsigned short readUShort(DATA_READER* reader)
{
	unsigned short value = *(const unsigned short*)(reader->data + reader->position);
	reader->position += sizeof(unsigned short);
	return value;
}

static short readShort(DATA_READER* reader)
{
	unsigned short value = *(const short*)(reader->data + reader->position);
	reader->position += sizeof(short);
	return value;
}

static void readData(DATA_READER* reader, void* data, unsigned int size, EXECUTE_RESULT* executeResult)
{
	if (reader->position + size > reader->size)
	{
		char errorMessage[2048];
		sprintf(errorMessage, "Insufficient amount of data requested. Requested %d bytes, but only %ld bytes are available.", size, reader->size - reader->position);
		executeResultFailed(executeResult, errorMessage);
		return;
	}
	
	memcpy(data, (reader->data + reader->position), size);
	reader->position += size;
	executeResultSucceeded(executeResult);
}


WAD* wadLoadFromWadData(const unsigned char* data, long dataSize, EXECUTE_RESULT* executeResult)
{
	DATA_READER _reader;
	_reader.data = data;
	_reader.size = dataSize;
	_reader.position = 0;
	DATA_READER* reader = &_reader;
	
	// Used for parsing some extracted buffer
	DATA_READER _bufferReader;
	DATA_READER* bufferReader = &_bufferReader;
	
	char errorMessage[2048];
	EXECUTE_RESULT readDataResult;
	WAD* wad = wadCreate();
	
	/// SECTION 1 – VERSION
	const unsigned int version = readUInt(reader);
	if (version < 129 /*|| version > 130*/)
	{
		sprintf(errorMessage, "Invalid file version: %d", version);
		executeResultFailed(executeResult, errorMessage);
		wadRelease(wad);
		return NULL;
	}
	wad->version = version;
	
	/// SECTION 2 – TEXTURES
	const unsigned int numTextureSamples = readUInt(reader);
	if (numTextureSamples > 4096)
	{
		sprintf(errorMessage, "Too much texture samples found in file: %d", numTextureSamples);
		executeResultFailed(executeResult, errorMessage);
		wadRelease(wad);
		return NULL;
	}
	wad->numTextureSamples = numTextureSamples;
	const unsigned int rawTextureSapmplesDataSize = sizeof(TEXTURE_SAMPLE) * numTextureSamples;
	wad->textureSamples = malloc(rawTextureSapmplesDataSize);
	if (rawTextureSapmplesDataSize == numTextureSamples * 8)
	{
		readData(reader, wad->textureSamples, rawTextureSapmplesDataSize, &readDataResult);
		if (!readDataResult.succeeded)
		{
			executeResultFailedCopy(executeResult, &readDataResult);
			wadRelease(wad);
			return NULL;
		}
	}
	else
	{
		for (int sampleIndex = 0; sampleIndex < numTextureSamples; sampleIndex++)
		{
			TEXTURE_SAMPLE* sample = &(wad->textureSamples[sampleIndex]);
			sample->x = readUChar(reader);
			sample->y = readUChar(reader);
			sample->page = readUShort(reader);
			sample->flipX = readChar(reader);
			sample->addW = readUChar(reader);
			sample->flipY = readChar(reader);
			sample->addH = readUChar(reader);
		}
	}
	
	const unsigned int numTextureBytes = readUInt(reader);
	const unsigned int numPagesBalance = numTextureBytes % (256 * 256 * 3);
	if (numPagesBalance > 0)
	{
		sprintf(errorMessage, "Incorrect size of texture data: %d. %d bytes are exceeded", numTextureBytes, numPagesBalance);
		executeResultFailed(executeResult, errorMessage);
		wadRelease(wad);
		return NULL;
	}
	const unsigned int mapHeightBalance = numTextureBytes % (256 * 3);
	if (numPagesBalance > 0)
	{
		sprintf(errorMessage, "Incorrect size of texture data: %d. %d bytes are exceeded", numTextureBytes, mapHeightBalance);
		executeResultFailed(executeResult, errorMessage);
		wadRelease(wad);
		return NULL;
	}
	
	const unsigned int numTexturePages = numTextureBytes / (256 * 256 * 3);
	wad->numTexturePages = numTexturePages;
	wad->texturePages = malloc(sizeof(TEXTURE_PAGE) * numTexturePages);
	for (int pageIndex = 0; pageIndex < numTexturePages; pageIndex++)
	{
		readData(reader, wad->texturePages[pageIndex].data, sizeof(wad->texturePages[pageIndex].data), &readDataResult);
		if (!readDataResult.succeeded)
		{
			executeResultFailedCopy(executeResult, &readDataResult);
			wadRelease(wad);
			return NULL;
		}
	}
	
	/// SECTION 3 – MESHES
	const unsigned int numMeshPointers = readUInt(reader);
	if (numMeshPointers > 4096)
	{
		sprintf(errorMessage, "Too much mesh pointers found in file: %d", numMeshPointers);
		executeResultFailed(executeResult, errorMessage);
		wadRelease(wad);
		return NULL;
	}
	const unsigned int meshPointersSize = numMeshPointers * 4;
	unsigned int* raw_meshPointersList = malloc(meshPointersSize);
	readData(reader, raw_meshPointersList, meshPointersSize, &readDataResult);
	if (!readDataResult.succeeded)
	{
		executeResultFailedCopy(executeResult, &readDataResult);
		wadRelease(wad);
		return NULL;
	}
	
	const unsigned int meshDataPackageSize = readUInt(reader) * 2;
	const long meshDataPackageEnd = reader->position + meshDataPackageSize;
	const unsigned int estimatedNumMeshes = meshDataPackageSize / sizeof(MESH);
	unsigned int numMeshes = 0;	// estimatedNumMeshes is always greater than numMeshes
	MESH* temporaryMeshes = malloc(sizeof(MESH) * estimatedNumMeshes);
	unsigned int* temporaryMeshAddresses = malloc(sizeof(unsigned int) * estimatedNumMeshes);
	const unsigned int meshDataPackageAddress = (unsigned int)reader->position;
	while (meshDataPackageEnd > reader->position)
	{
		// TODO: make error checks in this loop
		
		temporaryMeshAddresses[numMeshes] = (unsigned int)(reader->position - meshDataPackageAddress);
		
		MESH* mesh = &(temporaryMeshes[numMeshes]);
		numMeshes++;
		mesh->boundingSphere.cx = readShort(reader);
		mesh->boundingSphere.cy = readShort(reader);
		mesh->boundingSphere.cz = readShort(reader);
		mesh->boundingSphere.radius = readUShort(reader);
		mesh->boundingSphere.unknown = readUShort(reader);
		
		const unsigned short numVertices = readUShort(reader);
		VERTEX* vertices = malloc(sizeof(VERTEX) * numVertices);
		for (unsigned short vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
		{
			vertices[vertexIndex].vx = readUShort(reader);
			vertices[vertexIndex].vy = readUShort(reader);
			vertices[vertexIndex].vz = readUShort(reader);
		}
		mesh->numVertices = numVertices;
		mesh->vertices = vertices;
		
		const short numNormalsShades = readShort(reader);
		mesh->usesNormals = numNormalsShades > 0;
		if (mesh->usesNormals)
		{
			const unsigned short numNormals = numNormalsShades;
			if (numVertices != numNormals)
			{
				sprintf(errorMessage, "Number of vertices(%d) does not match with number of normals(%d)", numVertices, numNormals);
				executeResultFailed(executeResult, errorMessage);
				wadRelease(wad);
				return NULL;
			}
			
			for (unsigned int normalIndex = 0; normalIndex < numNormals; normalIndex++)
			{
				mesh->vertices[normalIndex].nx = readShort(reader);
				mesh->vertices[normalIndex].ny = readShort(reader);
				mesh->vertices[normalIndex].nz = readShort(reader);
				
				/*mesh->vertices[normalIndex].nx /= 16300;
				mesh->vertices[normalIndex].ny /= 16300;
				mesh->vertices[normalIndex].nz /= 16300;*/
			}
		}
		else
		{
			const unsigned short numShades = -numNormalsShades;
			if (numVertices != numShades)
			{
				sprintf(errorMessage, "Number of vertices(%d) does not match with number of shades(%d)", numVertices, numShades);
				executeResultFailed(executeResult, errorMessage);
				wadRelease(wad);
				return NULL;
			}
			
			for (unsigned int shadeIndex = 0; shadeIndex < numShades; shadeIndex++)
			{
				mesh->vertices[shadeIndex].shade = readShort(reader);
			}
		}
		
		const unsigned int numPolygons = readUShort(reader);
		mesh->numPolygons = numPolygons;
		mesh->polygons = malloc(sizeof(POLYGON) * numPolygons);
		unsigned int numQuads = 0;
		for (unsigned int polygonIndex = 0; polygonIndex < numPolygons; polygonIndex++)
		{
			mesh->polygons[polygonIndex].isTriangle = readUShort(reader) == 8 ? 1 : 0;
			mesh->polygons[polygonIndex].v1 = readUShort(reader);
			mesh->polygons[polygonIndex].v2 = readUShort(reader);
			mesh->polygons[polygonIndex].v3 = readUShort(reader);
			if (!mesh->polygons[polygonIndex].isTriangle)
			{
				mesh->polygons[polygonIndex].v4 = readUShort(reader);
				numQuads++;
			}
			mesh->polygons[polygonIndex].textureInfo = readUShort(reader);
			mesh->polygons[polygonIndex].attributes = readUChar(reader);
			mesh->polygons[polygonIndex].unknown = readUChar(reader);
		}
		if (numQuads % 2)
		{
			/*unsigned short padding = */
			readUShort(reader);
		}
	}
	wad->numMeshes = numMeshes;
	wad->meshes = malloc(sizeof(MESH) * numMeshes);
	unsigned int* raw_meshAddresses = malloc(sizeof(unsigned int) * numMeshes);
	for (unsigned int meshIndex = 0; meshIndex < numMeshes; meshIndex++)
	{
		wad->meshes[meshIndex] = temporaryMeshes[meshIndex];
		raw_meshAddresses[meshIndex] = temporaryMeshAddresses[meshIndex];
	}
	free(temporaryMeshAddresses);
	free(temporaryMeshes);
	
	/// SECTION 4 – ANIMATIONS
	const unsigned int numRawAnimations = readUInt(reader);
	const unsigned int rawAnimationsDataSize = sizeof(RAW_ANIMATION) * numRawAnimations;
	RAW_ANIMATION* rawAnimations = malloc(sizeof(RAW_ANIMATION) * numRawAnimations);
	
	if(rawAnimationsDataSize == numRawAnimations * 40)
	{
		readData(reader, rawAnimations, rawAnimationsDataSize, &readDataResult);
		if (!readDataResult.succeeded)
		{
			executeResultFailedCopy(executeResult, &readDataResult);
			wadRelease(wad);
			return NULL;
		}
	}
	else
	{
		for (unsigned int animationIndex = 0; animationIndex < numRawAnimations; animationIndex++)
		{
			RAW_ANIMATION* animation = &(rawAnimations[animationIndex]);
			animation->keyframeOffset = readUInt(reader);
			animation->frameDuration = readUChar(reader);
			animation->keyframeSize = readUChar(reader);
			animation->stateId = readUShort(reader);
			animation->unknown1 = readShort(reader);
			animation->speed = readShort(reader);
			animation->acceleration = readInt(reader);
			animation->unknown2 = readLong(reader);
			animation->frameStart = readUShort(reader);
			animation->frameEnd = readUShort(reader);
			animation->nextAnimation = readUShort(reader);
			animation->frameIn = readUShort(reader);
			animation->numStateChanges = readUShort(reader);
			animation->changesIndex = readUShort(reader);
			animation->numCommands = readUShort(reader);
			animation->commandOffsets = readUShort(reader);
		}
	}
	
	const unsigned int numStateChanges = readUInt(reader);
	RAW_STATE_CHANGE* rawStateChanges = malloc(sizeof(RAW_STATE_CHANGE) * numStateChanges);
	for (unsigned int stateChangeIndex = 0; stateChangeIndex < numStateChanges; stateChangeIndex++)
	{
		rawStateChanges[stateChangeIndex].stateId = readUShort(reader);
		rawStateChanges[stateChangeIndex].numDispatches = readUShort(reader);
		rawStateChanges[stateChangeIndex].dispatchesIndex = readUShort(reader);
	}
	
	const unsigned int numDispatches = readUInt(reader);
	RAW_DISPATCH* rawDispatches = malloc(sizeof(RAW_DISPATCH) * numDispatches);
	for (unsigned int dispatchIndex = 0; dispatchIndex < numDispatches; dispatchIndex++)
	{
		rawDispatches[dispatchIndex].inRange = readUShort(reader);
		rawDispatches[dispatchIndex].outRange = readUShort(reader);
		rawDispatches[dispatchIndex].nextAnim = readUShort(reader);
		rawDispatches[dispatchIndex].frameIn = readUShort(reader);
	}
	
	/* * * Commands * * */
	const unsigned int numCommandWords = readUInt(reader);
	const unsigned int commandBufferSize = numCommandWords * 2;
	unsigned char* commandBuffer = malloc(commandBufferSize);
	readData(reader, commandBuffer, commandBufferSize, &readDataResult);
	if (!readDataResult.succeeded)
	{
		// TODO: elegant release resources, not this ugly thing
		free(commandBuffer);
		free(rawDispatches);
		free(rawStateChanges);
		free(rawAnimations);
		free(raw_meshPointersList);
		
		executeResultFailedCopy(executeResult, &readDataResult);
		wadRelease(wad);
		return NULL;
	}
	
	/* * * Links * * */
	const unsigned int numLinkDWords = readUInt(reader);
	const unsigned int linkBufferSize = numLinkDWords * 4;
	unsigned char* linkBuffer = malloc(linkBufferSize);
	readData(reader, linkBuffer, linkBufferSize, &readDataResult);
	if (!readDataResult.succeeded)
	{
		free(linkBuffer);
		free(commandBuffer);
		free(rawDispatches);
		free(rawStateChanges);
		free(rawAnimations);
		free(raw_meshPointersList);
		
		executeResultFailedCopy(executeResult, &readDataResult);
		wadRelease(wad);
		return NULL;
	}
	
	const unsigned int numKeyframeWords = readUInt(reader);
	const unsigned int keyframeBufferSize = numKeyframeWords * 2;
	unsigned char* keyframeBuffer = malloc(keyframeBufferSize);
	readData(reader, keyframeBuffer, keyframeBufferSize, &readDataResult);
	if (!readDataResult.succeeded)
	{
		free(keyframeBuffer);
		free(linkBuffer);
		free(commandBuffer);
		free(rawDispatches);
		free(rawStateChanges);
		free(rawAnimations);
		free(raw_meshPointersList);
		
		executeResultFailedCopy(executeResult, &readDataResult);
		wadRelease(wad);
		return NULL;
	}
	
	/// SECTION 5 – MODELS
	const unsigned int numMovables = readUInt(reader);
	RAW_MOVABLE* rawMovables = malloc(sizeof(RAW_MOVABLE) * numMovables);
	for (unsigned int movableIndex = 0; movableIndex < numMovables; movableIndex++)
	{
		RAW_MOVABLE* movable = &(rawMovables[movableIndex]);
		movable->objectId = readUInt(reader);
		movable->numPointers = readUShort(reader);
		movable->pointersIndex = readUShort(reader);
		movable->linksIndex = readUInt(reader);
		movable->keyframesOffset = readUInt(reader);
		movable->animsIndex = readShort(reader);
	}
	
	const unsigned int numStatics = readUInt(reader);
	RAW_STATIC* statics = malloc(sizeof(RAW_STATIC) * numStatics);
	for (unsigned int staticIndex = 0; staticIndex < numStatics; staticIndex++)
	{
		RAW_STATIC* currentStatic = &(statics[staticIndex]);
		currentStatic->objectId = readUInt(reader);
		currentStatic->pointersIndex = readUShort(reader);
		currentStatic->vx1 = readShort(reader);
		currentStatic->vx2 = readShort(reader);
		currentStatic->vy1 = readShort(reader);
		currentStatic->vy2 = readShort(reader);
		currentStatic->vz1 = readShort(reader);
		currentStatic->vz2 = readShort(reader);
		currentStatic->cx1 = readShort(reader);
		currentStatic->cx2 = readShort(reader);
		currentStatic->cy1 = readShort(reader);
		currentStatic->cy2 = readShort(reader);
		currentStatic->cz1 = readShort(reader);
		currentStatic->cz2 = readShort(reader);
		currentStatic->flags = readUShort(reader);
	}
	
	/// PROCESS READ DATA
	
	wad->numMovables = numMovables;
	wad->movables = malloc(sizeof(MOVABLE) * wad->numMovables);
	
	/* * * Calculate movables information * * */
	
	const unsigned int estimatedNumSkeletons = numMovables;
	unsigned int numSkeletons = 0;
	SKELETON* estimatedSkeletons = malloc(sizeof(SKELETON) * estimatedNumSkeletons);
	
	unsigned int numWrittenSkeletonReferences = 0;
	unsigned int* skeletonReferences = malloc(sizeof(unsigned int) * estimatedNumSkeletons);
	
	for (unsigned int movableIndex = 0; movableIndex < numMovables; movableIndex++)
	{
		RAW_MOVABLE* rawMovable = &rawMovables[movableIndex];
		MOVABLE* movable = &wad->movables[movableIndex];
		
		movable->movableId = rawMovable->objectId;
		movable->numMeshes = rawMovable->numPointers;
		
		/* * * Get mesh indices (movable->meshIndices) for movable * * */
		movable->meshIndices = malloc(sizeof(unsigned int) * movable->numMeshes);
		for (unsigned int meshIndex = 0; meshIndex < movable->numMeshes; meshIndex++)
		{
			int found = 0;
			const unsigned int meshAddress = raw_meshPointersList[rawMovable->pointersIndex + meshIndex];
			for (unsigned int addressIndex = 0; addressIndex < wad->numMeshes; addressIndex++)
			{
				if (raw_meshAddresses[addressIndex] == meshAddress)
				{
					movable->meshIndices[meshIndex] = addressIndex;
					found = 1;
					break;
				}
			}
		}
		
		/* * * Get skeleton data (movable->skeletonIndex and wad->skeletons) for movable * * */
		unsigned int firstLinkAddress = rawMovable->linksIndex;
		int skeletonIndex = -1;
		for (int i = 0; i < numWrittenSkeletonReferences; i++)
		{
			if (skeletonReferences[i] == firstLinkAddress)
			{
				skeletonIndex = i;
				break;
			}
		}
		// if skeleton found then use it instead creation duplicate
		if (skeletonIndex >= 0)
		{
			movable->skeletonIndex = skeletonIndex;
		}
		else
		{
			movable->skeletonIndex = numSkeletons;

			_bufferReader.data = linkBuffer;
			_bufferReader.size = linkBufferSize;
			_bufferReader.position = firstLinkAddress * 4;	// just because it's measured in integers

			SKELETON* skeleton = &estimatedSkeletons[numSkeletons];
			skeleton->numJoints = movable->numMeshes - 1;
			skeleton->joints = malloc(sizeof(JOINT) * skeleton->numJoints);
			for (unsigned int jointIndex = 0; jointIndex < skeleton->numJoints; jointIndex++)
			{
				JOINT* currentJoint = &(skeleton->joints[jointIndex]);
				currentJoint->opCode = readInt(bufferReader);
				currentJoint->dx = readInt(bufferReader);
				currentJoint->dy = readInt(bufferReader);
				currentJoint->dz = readInt(bufferReader);
			}
			
			numSkeletons++;
		}
		
		/* * * Get animation data for movable (movable->animations) * * */
		movable->numAnimations = 0;
		unsigned int currentMovableIndex = movableIndex;
		while (rawMovable->animsIndex >= 0)
		{
			currentMovableIndex++;
			
			// If we reached the last movable, but number of animations is still unknown
			if (currentMovableIndex >= numMovables)
			{
				movable->numAnimations = numRawAnimations - rawMovable->animsIndex;
				break;
			}
			
			// If number of animations is correct
			if (rawMovables[currentMovableIndex].animsIndex >= 0)
			{
				movable->numAnimations = rawMovables[currentMovableIndex].animsIndex - rawMovable->animsIndex;
				break;
			}
		}
		
		if (movable->numAnimations > 0)
		{
			movable->animations = malloc(sizeof(ANIMATION) * movable->numAnimations);
			for (unsigned short animationIndex = 0; animationIndex < movable->numAnimations; animationIndex++)
			{
				ANIMATION* animation = &movable->animations[animationIndex];
				RAW_ANIMATION* rawAnimation = &rawAnimations[rawMovable->animsIndex + animationIndex];
				
				/* Get next animation to deternime the keyframe buffer size */
				RAW_ANIMATION* nextRawAnimation = NULL;
				short nextRawAnimationIndex = rawMovable->animsIndex + animationIndex;
				while (1)
				{
					nextRawAnimationIndex += 1;
					if (nextRawAnimationIndex >= numRawAnimations)
					{
						break;
					}
					
					if (rawAnimations[nextRawAnimationIndex].keyframeSize > 0)
					{
						nextRawAnimation = &(rawAnimations[nextRawAnimationIndex]);
						break;
					}
				}
				
				animation->stateId = rawAnimation->stateId;
				
				/* * * Get animation keyframes * * */
				if (rawAnimation->keyframeSize > 0)
				{
					if (nextRawAnimation)
					{
						animation->numKeyframes = (nextRawAnimation->keyframeOffset - rawAnimation->keyframeOffset) / (rawAnimation->keyframeSize * 2);
					}
					else
					{
						animation->numKeyframes = (numKeyframeWords * 2 - rawAnimation->keyframeOffset) / (rawAnimation->keyframeSize * 2);
					}
					
					_bufferReader.data = keyframeBuffer;
					_bufferReader.size = keyframeBufferSize;
					_bufferReader.position = rawAnimation->keyframeOffset;
					animation->keyframes = malloc(sizeof(KEYFRAME) * animation->numKeyframes);
					for (unsigned int keyframeIndex = 0; keyframeIndex < animation->numKeyframes; keyframeIndex++)
					{
						KEYFRAME* keyframe = &animation->keyframes[keyframeIndex];
						keyframe->bb1x = readShort(bufferReader);
						keyframe->bb2x = readShort(bufferReader);
						keyframe->bb1y = readShort(bufferReader);
						keyframe->bb2y = readShort(bufferReader);
						keyframe->bb1z = readShort(bufferReader);
						keyframe->bb2z = readShort(bufferReader);
						keyframe->offx = readShort(bufferReader);
						keyframe->offy = readShort(bufferReader);
						keyframe->offz = readShort(bufferReader);
						
						/*
						 Tunrs out, movable can have 123 joints
						 ANIMATION::keyframeSize is unsigned char, where max value is 255
						 9 words go for bounding box and offset
						 other 123 pairs (123*2=246) of words - for rotation
						 */
						
						// TODO: remove, use _bufferReader.position instead
						/* rawAnimation->keyframeSize represents maximum size of whole animation's keyframes.
						   Some keyframe sizes are less than others, so a garbage padding is added.
						 */
						short wordsLeft = (rawAnimation->keyframeSize - 9);
						
						keyframe->rotations = malloc(sizeof(ROTATION) * movable->numMeshes);
						memset(keyframe->rotations, 0, sizeof(ROTATION) * movable->numMeshes);
						
						for (unsigned int rotationIndex = 0; rotationIndex < movable->numMeshes; rotationIndex++)
						{
							ROTATION* rotation = &keyframe->rotations[rotationIndex];
							unsigned int angleSet = readUShort(bufferReader);
							wordsLeft--;
							unsigned int nextWord = 0;
							unsigned int axes = angleSet & 0xc000;
							
							switch (axes)
							{
								case ROTATION_USED_THREE_AXES:
									nextWord = readUShort(bufferReader);
									wordsLeft--;
									angleSet = angleSet * 0x10000 + nextWord;
									rotation->rotz = (angleSet & 0x3ff);// * 16;
									angleSet >>= 10;
									rotation->roty = (angleSet & 0x3ff);// * 16;
									angleSet >>= 10;
									rotation->rotx = (angleSet & 0x3ff);// * 16;
									break;

								case ROTATION_USED_X_AXIS:
									rotation->rotx = (angleSet & 0x3fff) / 4;
									rotation->roty = 0;
									rotation->rotz = 0;
									break;

								case ROTATION_USED_Y_AXIS:
									rotation->rotx = 0;
									rotation->roty = (angleSet & 0x3fff) / 4;
									rotation->rotz = 0;
									break;

								case ROTATION_USED_Z_AXIS:
									rotation->rotx = 0;
									rotation->roty = 0;
									rotation->rotz = (angleSet & 0x3fff) / 4;
									break;
												
								default:
									/* This never should happen */
									break;
							}
						}
						
						// Skip garbage padding
						while (wordsLeft > 0)
						{
							readUShort(bufferReader);
							wordsLeft--;
						}
					}
				}
				else
				{
					animation->numKeyframes = 0;
					animation->keyframes = NULL;
				}
				
				/* O-la la-la */
				
				animation->frameDuration = rawAnimation->frameDuration;
				animation->unknown1 = rawAnimation->unknown1;
				animation->speed = rawAnimation->speed;
				animation->acceleration = rawAnimation->acceleration;
				animation->unknown2 = rawAnimation->unknown2;
				animation->frameStart = rawAnimation->frameStart;
				animation->frameEnd = rawAnimation->frameEnd;
				animation->nextAnimation = rawAnimation->nextAnimation - rawMovable->animsIndex;
				animation->nextAnimationFrame = rawAnimation->frameIn;
				
				/* * * Get animation state changes * * */
				animation->numStateChanges = rawAnimation->numStateChanges;
				animation->stateChanges = malloc(sizeof(STATE_CHANGE) * animation->numStateChanges);
				memset(animation->stateChanges, 0, sizeof(STATE_CHANGE) * animation->numStateChanges);
				for (unsigned int stateChangeIndex = 0; stateChangeIndex < animation->numStateChanges; stateChangeIndex++)
				{
					STATE_CHANGE* stateChange = &(animation->stateChanges[stateChangeIndex]);
					RAW_STATE_CHANGE* rawStateChange = &(rawStateChanges[rawAnimation->changesIndex + stateChangeIndex]);
					stateChange->stateId = rawStateChange->stateId;
					stateChange->numDispatches = rawStateChange->numDispatches;
					if (stateChange->numDispatches > 0)
					{
						/* * * Get animation dispatches * * */
						stateChange->dispatches = malloc(sizeof(DISPATCH) * stateChange->numDispatches);
						for (unsigned int dispatchIndex = 0; dispatchIndex < stateChange->numDispatches; dispatchIndex++)
						{
							DISPATCH* dispatch = &(stateChange->dispatches[dispatchIndex]);
							RAW_DISPATCH* rawDispatch = &(rawDispatches[rawStateChange->dispatchesIndex + dispatchIndex]);
							dispatch->inRange = rawDispatch->inRange;
							dispatch->outRange = rawDispatch->outRange;
							dispatch->nextAnim = rawDispatch->nextAnim - rawMovable->animsIndex;
							dispatch->frameIn = rawDispatch->frameIn;
						}
					}
				}
				
				/* * * Get animation commands * * */
				_bufferReader.data = commandBuffer;
				_bufferReader.size = commandBufferSize;
				_bufferReader.position = rawAnimation->commandOffsets * 2;	// Because it is offset in words
				animation->numCommands = rawAnimation->numCommands;
				animation->commands = malloc(sizeof(COMMAND) * animation->numCommands);
				for (unsigned int commandIndex = 0; commandIndex < animation->numCommands; commandIndex++)
				{
					COMMAND* command = &(animation->commands[commandIndex]);
					command->commandType = readUShort(bufferReader);
					if (command->commandType == 1)
					{
						command->parameters.positionCommand.x = readShort(bufferReader);
						command->parameters.positionCommand.y = readShort(bufferReader);
						command->parameters.positionCommand.z = readShort(bufferReader);
					}
					else if (command->commandType == 2)
					{
						command->parameters.jumpCommand.xy = readShort(bufferReader);
						command->parameters.jumpCommand.yz = readShort(bufferReader);
					}
					else if (command->commandType == 5)
					{
						command->parameters.playSoundCommand.frame = readUShort(bufferReader);
						command->parameters.playSoundCommand.soundID = readUShort(bufferReader);
					}
					else if (command->commandType == 6)
					{
						command->parameters.actionCommand.frame = readUShort(bufferReader);
						command->parameters.actionCommand.action = readUShort(bufferReader);
					}
				}
			}
		}
		else
		{
			movable->animations = NULL;
		}
	}
	
	wad->numSkeletons = numSkeletons;
	wad->skeletons = malloc(sizeof(SKELETON) * numSkeletons);
	memcpy(wad->skeletons, estimatedSkeletons, sizeof(SKELETON) * numSkeletons);
	
	free(skeletonReferences);
	free(estimatedSkeletons);
	free(rawMovables);
	free(linkBuffer);
	free(keyframeBuffer);
	free(rawAnimations);
	free(rawDispatches);
	free(rawStateChanges);
	free(commandBuffer);
	
	/* * * Get static objects * * */
	wad->numStatics = numStatics;
	wad->statics = malloc(sizeof(STATIC) * numStatics);
	for (unsigned int staticIndex = 0; staticIndex < numStatics; staticIndex++)
	{
		STATIC* staticObject = &(wad->statics[staticIndex]);
		RAW_STATIC* rawStatic = &(statics[staticIndex]);
		
		staticObject->staticId = rawStatic->objectId;
		staticObject->vx1 = rawStatic->vx1;
		staticObject->vy1 = rawStatic->vy1;
		staticObject->vz1 = rawStatic->vz1;
		
		staticObject->vx2 = rawStatic->vx2;
		staticObject->vy2 = rawStatic->vy2;
		staticObject->vz2 = rawStatic->vz2;
		
		staticObject->cx1 = rawStatic->cx1;
		staticObject->cy1 = rawStatic->cy1;
		staticObject->cz1 = rawStatic->cz1;
		
		staticObject->cx2 = rawStatic->cx2;
		staticObject->cy2 = rawStatic->cy2;
		staticObject->cz2 = rawStatic->cz2;
		
		staticObject->flags = rawStatic->flags;
		
		int found = 0;
		const unsigned int meshAddress = raw_meshPointersList[rawStatic->pointersIndex];
		for (unsigned int meshIndex = 0; meshIndex < wad->numMeshes; meshIndex++)
		{
			if (raw_meshAddresses[meshIndex] == meshAddress)
			{
				staticObject->meshIndex = meshIndex;
				found = 1;
				break;
			}
		}
		
		if (!found)
		{
			found = 0;
		}
	}
	
	free(statics);
	free(raw_meshPointersList);	// ...
	free(raw_meshAddresses);
	
	executeResultSucceeded(executeResult);
	return wad;
}
