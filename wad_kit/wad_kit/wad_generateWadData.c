//
//  wad_generateWadData.c
//  wad_kit
//
//  Created by Евгений Лютц on 09.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#include "wad_interface.h"
#include "execute_result.h"
#include "joint.h"
#include "skeleton.h"
#include "mesh.h"
#include "static_object.h"
#include "wad.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct BUFFER_WRITER
{
	unsigned char* buffer;
	unsigned int size;
	unsigned int position;
}
BUFFER_WRITER;

static BUFFER_WRITER* createBufferWriter(unsigned int size)
{
	BUFFER_WRITER* writer = malloc(sizeof(BUFFER_WRITER));
	writer->buffer = malloc(size);
	writer->size = size;
	writer->position = 0;
	return writer;
}

static void releaseBufferWriter(BUFFER_WRITER* writer)
{
	free(writer->buffer);
	free(writer);
}

static void releaseBufferWriterExceptData(BUFFER_WRITER* writer)
{
	free(writer);
}

static void writeBuffer(BUFFER_WRITER* writer, unsigned char* buffer, unsigned int bufferSize, EXECUTE_RESULT* executeResult)
{
	if (writer == NULL)
	{
		executeResultFailed(executeResult, "Buffer writer is not created.");
		return;
	}
	
	if (buffer == NULL)
	{
		executeResultFailed(executeResult, "No data passed to write.");
		return;
	}
	
	if (bufferSize == 0)
	{
		executeResultFailed(executeResult, "Nothing to write.");
		return;
	}
	
	if (writer->position + bufferSize >= writer->size)
	{
		const unsigned int newDataSize = writer->size + bufferSize + 4096;
		unsigned char* newData = realloc(writer->buffer, newDataSize);
		if (newData == NULL)
		{
			executeResultFailed(executeResult, "Cannot reallocate memory.");
			return;
		}
		writer->buffer = newData;
		writer->size = newDataSize;
	}
	//*((unsigned int*)(writer->buffer + writer->position)) = value;
	memcpy(writer->buffer + writer->position, buffer, bufferSize);
	writer->position += bufferSize;
	executeResultSucceeded(executeResult);
}

#define defWriteFunc(funcName, dataType, dataTypeSize) \
static void funcName(BUFFER_WRITER* writer, dataType value, EXECUTE_RESULT* executeResult) \
{\
	if (writer == NULL)\
	{\
		executeResultFailed(executeResult, "Buffer writer is not created.");\
		return;\
	}\
	\
	if (writer->position + dataTypeSize >= writer->size)\
	{\
		const unsigned int newSize = writer->size + 4096;\
		unsigned char* data = realloc(writer->buffer, newSize);\
		if (data == NULL)\
		{\
			executeResultFailed(executeResult, "Cannot reallocate memory.");\
			return;\
		}\
		writer->buffer = data;\
		writer->size = newSize;\
	}\
	*((dataType*)(writer->buffer + writer->position)) = value;\
	writer->position += dataTypeSize;\
	executeResultSucceeded(executeResult);\
}

defWriteFunc(writeUInt, unsigned int, sizeof(unsigned int))
defWriteFunc(writeUShort, unsigned short, sizeof(unsigned short))
defWriteFunc(writeUChar, unsigned char, sizeof(unsigned char))

defWriteFunc(writeLong, long, 8)
defWriteFunc(writeInt, int, sizeof(int))
defWriteFunc(writeShort, short, sizeof(short))
defWriteFunc(writeChar, char, sizeof(char))

typedef struct WAD_DATABLOCK_SIZES
{
	unsigned int generalData;
	unsigned int texture;
	unsigned int meshPointers;
	unsigned int meshLinks;
	unsigned int mesh;
	unsigned int animation;
	unsigned int keyframe;
	unsigned int command;
	unsigned int stateChange;
	unsigned int dispatch;
	unsigned int movable;
	unsigned int staticObject;
}
WAD_DATABLOCK_SIZES;

static void _calculateEstimatedWadFileSize(WAD* wad, WAD_DATABLOCK_SIZES* sizes)
{
	// file version, number of texture samples, movables, etc
	sizes->generalData = 1024;
	
	// testure samples
	sizes->texture =  8 * wad->numTextureSamples;
	
	// texture pages
	sizes->texture += sizeof(unsigned char) * 256 * 256 * 3 * wad->numTexturePages;
	
	// meshes
	sizes->mesh = 0;
	for (unsigned int i = 0; i < wad->numMeshes; i++)
	{
		// bounding sphere
		sizes->mesh += 10;
		
		// vertices
		sizes->mesh += wad->meshes[i].numVertices * 14;
		
		// polygons
		sizes->mesh += wad->meshes[i].numPolygons * 20;	// 14 is maximum, but why not 20 :)
	}
	
	// movables
	sizes->meshPointers = 0;
	sizes->meshLinks = 0;
	sizes->movable = wad->numMovables * 20;
	sizes->animation = 0;
	sizes->keyframe = 0;
	sizes->command = 0;
	sizes->stateChange = 0;
	sizes->dispatch = 0;
	for (unsigned int i = 0; i < wad->numMovables; i++)
	{
		// skeleton (links)
		const unsigned int numJoints = wad->skeletons[wad->movables[i].skeletonIndex].numJoints + 1;
		sizes->meshLinks += numJoints * 20;	// 16, but again just 20 :)
		
		// mesh pointers
		sizes->meshPointers += numJoints * 4;
		
		// links pointers
		sizes->meshLinks += numJoints * 4;
		
		// animations
		sizes->animation += wad->movables[i].numAnimations * 40;
		for (unsigned int j = 0; j < wad->movables[i].numAnimations; j++)
		{
			// keyframes (bounding box and offset info + all joint transformations)
			sizes->keyframe += 20 + wad->movables[i].animations[j].numKeyframes * numJoints * 4;
			
			// commands
			sizes->command += wad->movables[i].animations[j].numCommands * 10;	// maximum 8, but 10 8)
			
			// state changes
			sizes->stateChange += wad->movables[i].animations[j].numStateChanges * 6;
			for (unsigned int k = 0; k < wad->movables[i].animations[j].numStateChanges; k++)
			{
				// dispatches
				sizes->dispatch += wad->movables[i].animations[j].stateChanges[k].numDispatches * 10;	// 8, but 10 :)
			}
		}
	}
	
	// statics
	sizes->staticObject = wad->numStatics * 32;
}

static BUFFER_WRITER* _createTexturePackageData(WAD* wad, WAD_DATABLOCK_SIZES* sizes, EXECUTE_RESULT* executeResult)
{
	EXECUTE_RESULT writeResult;
	BUFFER_WRITER* writer = createBufferWriter(sizes->texture);
	
	/* * * Texture samples * * */
	writeUInt(writer, wad->numTextureSamples, &writeResult);
	for (unsigned int i = 0; i < wad->numTextureSamples; i++)
	{
		TEXTURE_SAMPLE* sample = &(wad->textureSamples[i]);
		writeUChar(writer, sample->x, &writeResult);
		writeUChar(writer, sample->y, &writeResult);
		writeUShort(writer, sample->page, &writeResult);
		writeChar(writer, sample->flipX, &writeResult);
		writeUChar(writer, sample->addW, &writeResult);
		writeChar(writer, sample->flipY, &writeResult);
		writeUChar(writer, sample->addH, &writeResult);
	}
	
	// delete this
	if (!writeResult.succeeded)
	{
		executeResultFailed(executeResult, writeResult.message);
		return NULL;
	}
	
	/* * * Texture pages * * */
	// TODO: convert 256x256x3 to a constant?
	const unsigned int numTextureBytes = wad->numTexturePages * 256 * 256 * 3;
	writeUInt(writer, numTextureBytes, &writeResult);
	for (unsigned int i = 0; i < wad->numTexturePages; i++)
	{
		writeBuffer(writer, wad->texturePages[i].data, 256 * 256 * 3, &writeResult);
		if (!writeResult.succeeded)
		{
			executeResultFailed(executeResult, writeResult.message);
			return NULL;
		}
	}
	
	executeResultSucceeded(executeResult);
	
	return writer;
}

static BUFFER_WRITER* _createMeshBuffer(WAD* wad, WAD_DATABLOCK_SIZES* sizes, unsigned int** out_meshOffsets, EXECUTE_RESULT* executeResult)
{
	EXECUTE_RESULT writeResult;
	BUFFER_WRITER* writer = createBufferWriter(sizes->mesh);
	unsigned int* meshOffsets = malloc(sizeof(unsigned int) * wad->numMeshes);
	*out_meshOffsets = meshOffsets;
	
	for (unsigned int i = 0; i < wad->numMeshes; i++)
	{
		meshOffsets[i] = writer->position;
		
		MESH* mesh = &(wad->meshes[i]);
		writeShort(writer, mesh->cx, &writeResult);
		writeShort(writer, mesh->cy, &writeResult);
		writeShort(writer, mesh->cz, &writeResult);
		writeUShort(writer, mesh->radius, &writeResult);
		writeUShort(writer, mesh->unknown, &writeResult);
		
		writeUShort(writer, mesh->numVertices, &writeResult);
		for (unsigned short j = 0; j < mesh->numVertices; j++)
		{
			writeShort(writer, mesh->vertices[j].vx, &writeResult);
			writeShort(writer, mesh->vertices[j].vy, &writeResult);
			writeShort(writer, mesh->vertices[j].vz, &writeResult);
		}
		
		if (mesh->usesNormals)
		{
			writeShort(writer, mesh->numVertices, &writeResult);
			for (unsigned short j = 0; j < mesh->numVertices; j++)
			{
				writeShort(writer, mesh->vertices[j].nx, &writeResult);
				writeShort(writer, mesh->vertices[j].ny, &writeResult);
				writeShort(writer, mesh->vertices[j].nz, &writeResult);
			}
		}
		else
		{
			writeShort(writer, -mesh->numVertices, &writeResult);
			for (unsigned short j = 0; j < mesh->numVertices; j++)
			{
				writeShort(writer, mesh->vertices[j].shade, &writeResult);
			}
		}
		
		writeUShort(writer, mesh->numPolygons, &writeResult);
		unsigned short numQuads = 0;
		for (unsigned short j = 0; j < mesh->numPolygons; j++)
		{
			POLYGON* polygon = &(mesh->polygons[j]);
			writeUShort(writer, polygon->isTriangle ? 8 : 9, &writeResult);
			writeUShort(writer, polygon->v1, &writeResult);
			writeUShort(writer, polygon->v2, &writeResult);
			writeUShort(writer, polygon->v3, &writeResult);
			if (!polygon->isTriangle)
			{
				writeUShort(writer, polygon->v4, &writeResult);
				numQuads++;
			}
			writeUShort(writer, polygon->textureInfo, &writeResult);
			writeUChar(writer, polygon->attributes, &writeResult);
			writeUChar(writer, polygon->unknown, &writeResult);
		}
		if (numQuads % 2)
		{
			writeUShort(writer, 0, &writeResult);
		}
	}
	
	executeResultSucceeded(executeResult);
	return writer;
}

static BUFFER_WRITER* _createAnimationsPackage(WAD* wad, WAD_DATABLOCK_SIZES* sizes, /* out */ short** out_animationIndices, /* out */ unsigned int** out_keyframeOffsets, /* out */ unsigned int** out_linkOffsets, /* out */ EXECUTE_RESULT* executeResult)
{
	EXECUTE_RESULT writeResult;
	BUFFER_WRITER* keyframeBuffer = createBufferWriter(sizes->keyframe);
	BUFFER_WRITER* linksBuffer = createBufferWriter(sizes->meshLinks);
	BUFFER_WRITER* commandsBuffer = createBufferWriter(sizes->command);
	BUFFER_WRITER* dispatchBuffer = createBufferWriter(sizes->dispatch);
	BUFFER_WRITER* stateChangeBuffer = createBufferWriter(sizes->stateChange);
	BUFFER_WRITER* animationBuffer = createBufferWriter(sizes->animation);
	
	short* animationIndices = malloc(sizeof(short) * wad->numMovables);
	*out_animationIndices = animationIndices;
	
	unsigned int* keyframeOffsets = malloc(sizeof(unsigned int) * wad->numMovables);
	*out_keyframeOffsets = keyframeOffsets;
	
	unsigned int* linkOffsets = malloc(sizeof(unsigned int) * wad->numSkeletons);
	*out_linkOffsets = linkOffsets;
	
	unsigned short numAnimations = 0;
	unsigned int currentKeyframeOffset = 0;
	unsigned short numDispatches = 0;
	unsigned int numStateChanges = 0;
	for (unsigned int i = 0; i < wad->numMovables; i++)
	{
		MOVABLE* movable = &(wad->movables[i]);
		
		/* * * Animations data package */
		if (movable->numAnimations > 0)
		{
			animationIndices[i] = numAnimations;
		}
		else
		{
			animationIndices[i] = -1;
		}
		keyframeOffsets[i] = currentKeyframeOffset;
		for (unsigned int j = 0; j < movable->numAnimations; j++)
		{
			ANIMATION* animation = &(movable->animations[j]);
			
			/* 10 should be 9 (bounding box - 6 bytes, offset - 3 bytes), but the size of keyframe buffer is expressed in words, thus the value should be divided by 2, so we add extra 1 byte to make value even. */
			//const unsigned int keyframeSize = 10 + movable->numMeshes * 2;
			const unsigned int keyframeSize = 9 + (movable->numMeshes * 2);
			
			writeUInt(animationBuffer, currentKeyframeOffset, &writeResult);
			writeUChar(animationBuffer, animation->frameDuration, &writeResult);
			writeUChar(animationBuffer, keyframeSize, &writeResult);
			writeUShort(animationBuffer, animation->stateId, &writeResult);
			writeShort(animationBuffer, animation->unknown1, &writeResult);
			writeShort(animationBuffer, animation->speed, &writeResult);
			writeInt(animationBuffer, animation->acceleration, &writeResult);
			writeLong(animationBuffer, animation->unknown2, &writeResult);
			writeUShort(animationBuffer, animation->frameStart, &writeResult);
			writeUShort(animationBuffer, animation->frameEnd, &writeResult);
			writeUShort(animationBuffer, numAnimations + animation->nextAnimation, &writeResult);
			writeUShort(animationBuffer, animation->nextAnimationFrame, &writeResult);
			writeUShort(animationBuffer, animation->numStateChanges, &writeResult);
			writeUShort(animationBuffer, numStateChanges, &writeResult);
			writeUShort(animationBuffer, animation->numCommands, &writeResult);
			writeUShort(animationBuffer, commandsBuffer->position / 2, &writeResult);
			
			/* * * Keyframes data package * * */
			for (unsigned int k = 0; k < animation->numKeyframes; k++)
			{
				KEYFRAME* keyframe = &(animation->keyframes[k]);
				
				writeShort(keyframeBuffer, keyframe->bb1x, &writeResult);
				writeShort(keyframeBuffer, keyframe->bb2x, &writeResult);
				writeShort(keyframeBuffer, keyframe->bb1y, &writeResult);
				writeShort(keyframeBuffer, keyframe->bb2y, &writeResult);
				writeShort(keyframeBuffer, keyframe->bb1z, &writeResult);
				writeShort(keyframeBuffer, keyframe->bb2z, &writeResult);
				writeShort(keyframeBuffer, keyframe->offx, &writeResult);
				writeShort(keyframeBuffer, keyframe->offy, &writeResult);
				writeShort(keyframeBuffer, keyframe->offz, &writeResult);
				
				for (unsigned int l = 0; l < movable->numMeshes; l++)
				{
					ROTATION* r = &(keyframe->rotations[l]);
					
					const unsigned int value =
					(((unsigned int)r->rotx) << 20) |
					(((unsigned int)r->roty) << 10) |
					((unsigned int)r->rotz);

					const unsigned short* v1 = (unsigned short*)&value;
					writeUShort(keyframeBuffer, v1[1], &writeResult);
					writeUShort(keyframeBuffer, v1[0], &writeResult);
				}
			}
			currentKeyframeOffset += keyframeSize * 2 * animation->numKeyframes;

			/* * * Commands data package * * */
			for (unsigned int k = 0; k < animation->numCommands; k++)
			{
				COMMAND* command = &(animation->commands[k]);
				writeUShort(commandsBuffer, command->commandType, &writeResult);
				if (command->commandType == 1)
				{
					writeShort(commandsBuffer, command->parameters.positionCommand.x, &writeResult);
					writeShort(commandsBuffer, command->parameters.positionCommand.y, &writeResult);
					writeShort(commandsBuffer, command->parameters.positionCommand.z, &writeResult);
				}
				else if (command->commandType == 2)
				{
					writeShort(commandsBuffer, command->parameters.jumpCommand.xy, &writeResult);
					writeShort(commandsBuffer, command->parameters.jumpCommand.yz, &writeResult);
				}
				else if (command->commandType == 5)
				{
					writeUShort(commandsBuffer, command->parameters.playSoundCommand.frame, &writeResult);
					writeUShort(commandsBuffer, command->parameters.playSoundCommand.soundID, &writeResult);
				}
				else if (command->commandType == 6)
				{
					writeUShort(commandsBuffer, command->parameters.actionCommand.frame, &writeResult);
					writeUShort(commandsBuffer, command->parameters.actionCommand.action, &writeResult);
				}
			}
			
			/* * * State changes data package * * */
			for (unsigned int k = 0; k < animation->numStateChanges; k++)
			{
				STATE_CHANGE* stateChange = &(animation->stateChanges[k]);
				writeUShort(stateChangeBuffer, stateChange->stateId, &writeResult);
				writeUShort(stateChangeBuffer, stateChange->numDispatches, &writeResult);
				writeUShort(stateChangeBuffer, numDispatches, &writeResult);
				
				/* * * Dispatches data package * * */
				for (unsigned int l = 0; l < stateChange->numDispatches; l++)
				{
					DISPATCH* dispatch = &(stateChange->dispatches[l]);
					writeUShort(dispatchBuffer, dispatch->inRange, &writeResult);
					writeUShort(dispatchBuffer, dispatch->outRange, &writeResult);
					writeUShort(dispatchBuffer, dispatch->nextAnim + numAnimations, &writeResult);
					writeUShort(dispatchBuffer, dispatch->frameIn, &writeResult);
				}
				numDispatches += stateChange->numDispatches;
			}
			numStateChanges += animation->numStateChanges;
		}
		numAnimations += movable->numAnimations;
	}
	
	/* * * Links data package */
	for (unsigned int i = 0; i < wad->numSkeletons; i++)
	{
		linkOffsets[i] = linksBuffer->position / 4;	// just because it's measured in integers

		SKELETON* skeleton = &(wad->skeletons[i]);
		for (unsigned int j = 0; j < skeleton->numJoints; j++)
		{
			JOINT* joint = &(skeleton->joints[j]);
			writeInt(linksBuffer, joint->opCode, &writeResult);
			writeInt(linksBuffer, joint->dx, &writeResult);
			writeInt(linksBuffer, joint->dy, &writeResult);
			writeInt(linksBuffer, joint->dz, &writeResult);
		}
	}
	
	BUFFER_WRITER* animationPackage = createBufferWriter(4096 + animationBuffer->position + stateChangeBuffer->position + dispatchBuffer->position + commandsBuffer->position + linksBuffer->position + keyframeBuffer->position);
	
	// Animations
	writeUInt(animationPackage, numAnimations, &writeResult);
	writeBuffer(animationPackage, animationBuffer->buffer, animationBuffer->position, &writeResult);
	releaseBufferWriter(animationBuffer);
	
	// State changes
	writeUInt(animationPackage, numStateChanges, &writeResult);
	writeBuffer(animationPackage, stateChangeBuffer->buffer, stateChangeBuffer->position, &writeResult);
	releaseBufferWriter(stateChangeBuffer);
	
	// Dispatches
	writeUInt(animationPackage, numDispatches, &writeResult);
	writeBuffer(animationPackage, dispatchBuffer->buffer, dispatchBuffer->position, &writeResult);
	releaseBufferWriter(dispatchBuffer);
	
	// Commands
	writeUInt(animationPackage, commandsBuffer->position / 2, &writeResult);
	writeBuffer(animationPackage, commandsBuffer->buffer, commandsBuffer->position, &writeResult);
	releaseBufferWriter(commandsBuffer);
	
	// Links
	writeUInt(animationPackage, linksBuffer->position / 4, &writeResult);
	writeBuffer(animationPackage, linksBuffer->buffer, linksBuffer->position, &writeResult);
	releaseBufferWriter(linksBuffer);
	
	// Keyframes
	writeUInt(animationPackage, keyframeBuffer->position / 2, &writeResult);
	writeBuffer(animationPackage, keyframeBuffer->buffer, keyframeBuffer->position, &writeResult);
	releaseBufferWriter(keyframeBuffer);
	
	executeResultSucceeded(executeResult);
	return animationPackage;
}

static BUFFER_WRITER* _createModelsPackage(WAD* wad, WAD_DATABLOCK_SIZES* sizes, BUFFER_WRITER** out_meshPointers, unsigned int* meshOffsets, unsigned int* meshLinks, short* animationIndices, unsigned int* keyframeOffsets, EXECUTE_RESULT* executeResult)
{
	EXECUTE_RESULT writeResult;
	
	BUFFER_WRITER* meshPointers = createBufferWriter(sizes->meshPointers);
	*out_meshPointers = meshPointers;
	
	/* * * Movables * * */
	BUFFER_WRITER* movablesBuffer = createBufferWriter(sizes->movable);
	for (unsigned int i = 0; i < wad->numMovables; i++)
	{
		MOVABLE* movable = &(wad->movables[i]);
		writeUInt(movablesBuffer, movable->movableId, &writeResult);
		writeUShort(movablesBuffer, (unsigned short)movable->numMeshes, &writeResult);
		
		/* * * Mesh pointers nightmare (also continues in _createStaticsPackage) * * */
		writeUShort(movablesBuffer, (unsigned short)meshPointers->position / 4, &writeResult);
		for (unsigned int j = 0; j < movable->numMeshes; j++)
		{
			writeUInt(meshPointers, meshOffsets[movable->meshIndices[j]], &writeResult);
		}
		
		/* * * Links indices nightmare * * */
		writeUInt(movablesBuffer, meshLinks[movable->skeletonIndex], &writeResult);
		
		// TODO: implement
		writeUInt(movablesBuffer, keyframeOffsets[i], &writeResult);
		writeShort(movablesBuffer, animationIndices[i], &writeResult);
	}
	
	/* * * Statics * * */
	BUFFER_WRITER* staticsBuffer = createBufferWriter(sizes->staticObject);
	for (unsigned int i = 0; i < wad->numStatics; i++)
	{
		STATIC* staticObject = &(wad->statics[i]);
		writeUInt(staticsBuffer, staticObject->staticId, &writeResult);
		writeUShort(staticsBuffer, (unsigned short)meshPointers->position / 4, &writeResult);
		writeUInt(meshPointers, meshOffsets[staticObject->meshIndex], &writeResult);
		writeShort(staticsBuffer, staticObject->vx1, &writeResult);
		writeShort(staticsBuffer, staticObject->vx2, &writeResult);
		writeShort(staticsBuffer, staticObject->vy1, &writeResult);
		writeShort(staticsBuffer, staticObject->vy2, &writeResult);
		writeShort(staticsBuffer, staticObject->vz1, &writeResult);
		writeShort(staticsBuffer, staticObject->vz2, &writeResult);
		writeShort(staticsBuffer, staticObject->cx1, &writeResult);
		writeShort(staticsBuffer, staticObject->cx2, &writeResult);
		writeShort(staticsBuffer, staticObject->cy1, &writeResult);
		writeShort(staticsBuffer, staticObject->cy2, &writeResult);
		writeShort(staticsBuffer, staticObject->cz1, &writeResult);
		writeShort(staticsBuffer, staticObject->cz2, &writeResult);
		writeUShort(staticsBuffer, staticObject->flags, &writeResult);
	}
	
	BUFFER_WRITER* modelsPackage = createBufferWriter(4096 + movablesBuffer->position + staticsBuffer->position);
	
	// Movables
	writeUInt(modelsPackage, wad->numMovables, &writeResult);
	writeBuffer(modelsPackage, movablesBuffer->buffer, movablesBuffer->position, &writeResult);
	releaseBufferWriter(movablesBuffer);
	
	// Statics
	writeUInt(modelsPackage, wad->numStatics, &writeResult);
	writeBuffer(modelsPackage, staticsBuffer->buffer, staticsBuffer->position, &writeResult);
	releaseBufferWriter(staticsBuffer);
	
	executeResultSucceeded(executeResult);
	return modelsPackage;
}

unsigned char* wadGenerateWadData(WAD* wad, long* dataSize, EXECUTE_RESULT* executeResult)
{
	/* * * Calculate estimated file size * * */
	WAD_DATABLOCK_SIZES sizes = { };
	_calculateEstimatedWadFileSize(wad, &sizes);
	
	EXECUTE_RESULT writeResult;
	
	/* * * File version * * */
	const unsigned int fileVersion = wad->version;
	
	// DONE
	/* * * Texture package * * */
	BUFFER_WRITER* texturePackage = _createTexturePackageData(wad, &sizes, &writeResult);
	
	// DONE
	/* * * Meshes * * */
	unsigned int* meshOffsets = NULL;	// index = mesh index; value = offset in memory (in bytes)
	BUFFER_WRITER* meshBuffer = _createMeshBuffer(wad, &sizes, &meshOffsets, &writeResult);
	
	// DONE
	/* * * Animations package * * */
	unsigned int* keyframeOffsets = NULL;	// index = movable's index; value = keyframe's offset in keyframes package
	short* animationIndices = NULL;			// index = movable's index; value = animation index in animations package
	unsigned int* linkOffsets = NULL;		// index = skeleton's index; value = link offset in memory (in integers)
	BUFFER_WRITER* animationsPackage = _createAnimationsPackage(wad, &sizes, &animationIndices, &keyframeOffsets, &linkOffsets, &writeResult);
	
	// DONE
	/* * * Models package * * */
	BUFFER_WRITER* meshPointersBuffer = NULL;
	BUFFER_WRITER* modelsPackage = _createModelsPackage(wad, &sizes, &meshPointersBuffer, meshOffsets, linkOffsets, animationIndices, keyframeOffsets, &writeResult);
	
	// release temporary resources
	free(animationIndices);
	free(keyframeOffsets);
	free(linkOffsets);
	free(meshOffsets);
	
	
	BUFFER_WRITER* writer = createBufferWriter(4096 + texturePackage->position + meshPointersBuffer->position + meshBuffer->position + animationsPackage->position + modelsPackage->position);
	
	// File version
	writeUInt(writer, fileVersion, &writeResult);
	
	// Textures
	writeBuffer(writer, texturePackage->buffer, texturePackage->position, &writeResult);
	releaseBufferWriter(texturePackage);
	
	// Mesh pointers
	writeUInt(writer, meshPointersBuffer->position / 4, &writeResult);
	writeBuffer(writer, meshPointersBuffer->buffer, meshPointersBuffer->position, &writeResult);
	releaseBufferWriter(meshPointersBuffer);
	
	// Meshes
	writeUInt(writer, meshBuffer->position / 2, &writeResult);
	writeBuffer(writer, meshBuffer->buffer, meshBuffer->position, &writeResult);
	releaseBufferWriter(meshBuffer);
	
	// Animations
	writeBuffer(writer, animationsPackage->buffer, animationsPackage->position, &writeResult);
	releaseBufferWriter(animationsPackage);
	
	// Models
	writeBuffer(writer, modelsPackage->buffer, modelsPackage->position, &writeResult);
	releaseBufferWriter(modelsPackage);
	
	
	unsigned char* data = writer->buffer;
	*dataSize = writer->position;
	releaseBufferWriterExceptData(writer);
	return data;
}
