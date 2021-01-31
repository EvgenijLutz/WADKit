//
//  vertex_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef vertex_interface_h
#define vertex_interface_h

#include "wad_kit.h"

void vertexInitializeWithRawPosition(WK_VERTEX* vertex, short vx, short vy, short vz);
void vertexSetRawPosition(WK_VERTEX* vertex, short vx, short vy, short vz);
void vertexSetRawNormal(WK_VERTEX* vertex, short nx, short ny, short nz);
void vertexSetRawShade(WK_VERTEX* vertex, short shade);

vector3f vertexGetPosition(WK_VERTEX* vertex);

#endif /* vertex_interface_h */
