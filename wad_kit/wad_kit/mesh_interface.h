//
//  mesh_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef mesh_interface_h
#define mesh_interface_h

#include "wad_kit.h"

void meshInitializeFromBuffer(WK_MESH* mesh, WK_WAD* wad, BUFFER_READER* buffer, EXECUTE_RESULT* executeResult);
void meshDeinitialize(WK_MESH* mesh);

WK_WAD* meshGetWad(WK_MESH* mesh);

int meshUsesNormals(WK_MESH* mesh);
void meshSetUsesNormals(WK_MESH* mesh, int uses);

unsigned int meshGetNumVertices(WK_MESH* mesh);
WK_VERTEX meshGetVertex(WK_MESH* mesh, unsigned int vertexIndex);
void meshAddRawVertex(WK_MESH* mesh, unsigned short vx, unsigned short vy, unsigned short vz);
void meshSetRawVertexNormal(WK_MESH* mesh, unsigned int vertexIndex, short nx, short ny, short nz);
void meshSetRawVertexShade(WK_MESH* mesh, unsigned int vertexIndex, short shade);

unsigned int meshGetNumPolygons(WK_MESH* mesh);
WK_POLYGON* meshGetPolygon(WK_MESH* mesh, unsigned int polygonIndex);

#endif /* mesh_interface_h */
