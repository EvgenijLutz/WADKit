//
//  wad_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef wad_interface_h
#define wad_interface_h

#include "wad_kit.h"

WK_WAD* wadCreate(void);
void wadRelease(WK_WAD* wad);

WK_WAD* wadCreateFromContentsOfResourceFile(WK_SYSTEM* system, const char* name, EXECUTE_RESULT* executeResult);

unsigned int wadGetVersion(WK_WAD* wad);

unsigned int wadGetNumTexturePages(WK_WAD* wad);
TEXTURE_PAGE* wadGetTexturePage(WK_WAD* wad, unsigned int texturePageIndex);
unsigned char* wadGetTexturePageData(WK_WAD* wad, unsigned int texturePageIndex);

unsigned int wadGetNumMeshes(WK_WAD* wad);
MESH* wadGetMesh(WK_WAD* wad, unsigned int meshIndex);

unsigned int wadGetNumMovables(WK_WAD* wad);
MOVABLE* wadGetMovableByIndex(WK_WAD* wad, unsigned int movableIndex);
MOVABLE* wadGetMovableById(WK_WAD* wad, MOVABLE_ID movableId);

unsigned int wadGetNumStatics(WK_WAD* wad);
STATIC* wadGetStaticByIndex(WK_WAD* wad, unsigned int staticIndex);
STATIC* wadGetStaticById(WK_WAD* wad, STATIC_ID staticId);

#endif /* wad_interface_h */
