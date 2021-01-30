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

void texturePageInitializeFromBuffer(WK_TEXTURE_PAGE* texturePage, WK_WAD* wad, BUFFER_READER* buffer, EXECUTE_RESULT* executeResult);
void texturePageInitializeEmpty(WK_TEXTURE_PAGE* texturePage, WK_WAD* wad);
void texturePageInitializeWithData(WK_TEXTURE_PAGE* texturePage, WK_WAD* wad, const char* data);
void texturePageDeinitialize(WK_TEXTURE_PAGE* texturePage);

#endif /* texture_page_private_interface_h */
