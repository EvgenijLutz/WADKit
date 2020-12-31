//
//  texture_page_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef texture_page_private_interface_h
#define texture_page_private_interface_h

#include "texture_page_private_interface.h"

void texturePageInitializeFromBuffer(TEXTURE_PAGE* texturePage, WK_WAD* wad, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
void texturePageInitializeEmpty(TEXTURE_PAGE* texturePage, WK_WAD* wad);
void texturePageInitializeWithData(TEXTURE_PAGE* texturePage, WK_WAD* wad, const char* data);
void texturePageDeinitialize(TEXTURE_PAGE* texturePage);

#endif /* texture_page_private_interface_h */
