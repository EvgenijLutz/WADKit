//
//  wad_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef wad_interface_h
#define wad_interface_h

#include "texture.h"

typedef struct EXECUTE_RESULT EXECUTE_RESULT;
typedef struct WAD WAD;
typedef struct MESH MESH;
typedef struct STATIC STATIC;

WAD* wadCreate(void);
void wadRelease(WAD* wad);

WAD* wadLoadFromWadData(const unsigned char* data, long dataSize, EXECUTE_RESULT* executeResult);
unsigned char* wadGenerateWadData(WAD* wad, long* dataSize, EXECUTE_RESULT* executeResult);

unsigned int wadGetVersion(WAD* wad);

unsigned int wadGetNumTexturePages(WAD* wad);
unsigned char* wadGetTexturePageData(WAD* wad, unsigned int texturePageIndex);

unsigned int wadGetNumTextureSamples(WAD* wad);
TEXTURE_SAMPLE wadGetTextureSample(WAD* wad, unsigned int textureSampleIndex);

unsigned int wadGetNumMeshes(WAD* wad);
MESH* wadGetMesh(WAD* wad, unsigned int meshIndex);

unsigned int wadGetNumStatics(WAD* wad);
STATIC* wadGetStaticByIndex(WAD* wad, unsigned int staticIndex);
STATIC* wadGetStaticById(WAD* wad, unsigned int staticId);

#endif /* wad_interface_h */
