//
//  wad_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_private_interface_h
#define wad_private_interface_h

#include "private_interface.h"

WK_MESH* _wad_findMesh(unsigned short pointerListStart, unsigned short index, WK_WAD_LOAD_INFO* loadInfo);

#endif /* wad_private_interface_h */
