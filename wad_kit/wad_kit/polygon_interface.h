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

void polygonInitializeRawFromBuffer(POLYGON* polygon, MESH* mesh, BUFFER_READER* buffer, EXECUTE_RESULT* executeResult);

int polygonIsTriangle(POLYGON* polygon);
VERTEX* polygonGetVertex1(POLYGON* polygon);
VERTEX* polygonGetVertex2(POLYGON* polygon);
VERTEX* polygonGetVertex3(POLYGON* polygon);
VERTEX* polygonGetVertex4(POLYGON* polygon);

WK_TEXTURE_SAMPLE* polygonGetTextureSample(POLYGON* polygon);
unsigned short polygonGetTextureSampleShape(POLYGON* polygon);
unsigned short polygonIsFlipped(POLYGON* polygon);

#endif /* polygon_interface_h */
