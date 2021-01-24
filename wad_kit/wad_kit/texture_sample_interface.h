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

unsigned char textureSampleGetX(TEXTURE_SAMPLE* textureSample);
unsigned char textureSampleGetY(TEXTURE_SAMPLE* textureSample);
unsigned char textureSampleGetAddX(TEXTURE_SAMPLE* textureSample);
unsigned char textureSampleGetAddY(TEXTURE_SAMPLE* textureSample);
char textureSampleGetFlipX(TEXTURE_SAMPLE* textureSample);
char textureSampleGetFlipY(TEXTURE_SAMPLE* textureSample);
TEXTURE_PAGE* textureSampleGetTexturePage(TEXTURE_SAMPLE* textureSample);
vector2f textureSampleGetUVLocation(TEXTURE_SAMPLE* textureSample);
vector2f textureSampleGetUVSize(TEXTURE_SAMPLE* textureSample);

#endif /* texture_sample_interface_h */
