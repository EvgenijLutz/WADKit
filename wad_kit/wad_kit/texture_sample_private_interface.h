//
//  texture_sample_private_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 30.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef texture_sample_private_interface_h
#define texture_sample_private_interface_h

#include "private_interface.h"

void textureSampleInitializeFromBuffer(WK_TEXTURE_SAMPLE* textureSample, WK_WAD* wad, BUFFER_READER* buffer, EXECUTE_RESULT* executeResult);
void textureSampleDeinitialize(WK_TEXTURE_SAMPLE* textureSample);

#endif /* texture_sample_private_interface_h */
