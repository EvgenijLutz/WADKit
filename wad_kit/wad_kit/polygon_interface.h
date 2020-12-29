//
//  polygon_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef polygon_interface_h
#define polygon_interface_h

#include "wad_kit.h"

void polygonInitializeRawFromBuffer(POLYGON* polygon, MESH* mesh, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);

#endif /* polygon_interface_h */
