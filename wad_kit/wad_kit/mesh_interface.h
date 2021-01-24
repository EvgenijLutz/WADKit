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

void meshInitializeFromBuffer(MESH* mesh, WK_WAD* wad, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
void meshDeinitialize(MESH* mesh);

WK_WAD* meshGetWad(MESH* mesh);

int meshUsesNormals(MESH* mesh);
void meshSetUsesNormals(MESH* mesh, int uses);

unsigned int meshGetNumVertices(MESH* mesh);
VERTEX meshGetVertex(MESH* mesh, unsigned int vertexIndex);
void meshAddRawVertex(MESH* mesh, unsigned short vx, unsigned short vy, unsigned short vz);
void meshSetRawVertexNormal(MESH* mesh, unsigned int vertexIndex, short nx, short ny, short nz);
void meshSetRawVertexShade(MESH* mesh, unsigned int vertexIndex, short shade);

unsigned int meshGetNumPolygons(MESH* mesh);
POLYGON* meshGetPolygon(MESH* mesh, unsigned int polygonIndex);

#endif /* mesh_interface_h */
