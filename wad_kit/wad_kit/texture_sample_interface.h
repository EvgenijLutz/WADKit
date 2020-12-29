//
//  texture_sample_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef texture_sample_interface_h
#define texture_sample_interface_h

#include "wad_kit.h"

void textureSampleInitializeFromBuffer(TEXTURE_SAMPLE* textureSample, WK_WAD* wad, WK_BUFFER* buffer, EXECUTE_RESULT* executeResult);
void textureSampleDeinitialize(TEXTURE_SAMPLE* textureSample);

#endif /* texture_sample_interface_h */
