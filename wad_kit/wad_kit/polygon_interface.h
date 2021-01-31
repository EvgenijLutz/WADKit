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

void polygonInitializeRawFromBuffer(WK_POLYGON* polygon, WK_MESH* mesh, BUFFER_READER* buffer, EXECUTE_RESULT* executeResult);

int polygonIsTriangle(WK_POLYGON* polygon);
WK_VERTEX* polygonGetVertex1(WK_POLYGON* polygon);
WK_VERTEX* polygonGetVertex2(WK_POLYGON* polygon);
WK_VERTEX* polygonGetVertex3(WK_POLYGON* polygon);
WK_VERTEX* polygonGetVertex4(WK_POLYGON* polygon);

WK_TEXTURE_SAMPLE* polygonGetTextureSample(WK_POLYGON* polygon);
unsigned short polygonGetTextureSampleShape(WK_POLYGON* polygon);
unsigned short polygonIsFlipped(WK_POLYGON* polygon);

#endif /* polygon_interface_h */
