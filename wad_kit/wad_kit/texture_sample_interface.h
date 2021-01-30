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

unsigned char textureSampleGetX(WK_TEXTURE_SAMPLE* textureSample);
unsigned char textureSampleGetY(WK_TEXTURE_SAMPLE* textureSample);
unsigned char textureSampleGetAddX(WK_TEXTURE_SAMPLE* textureSample);
unsigned char textureSampleGetAddY(WK_TEXTURE_SAMPLE* textureSample);
char textureSampleGetFlipX(WK_TEXTURE_SAMPLE* textureSample);
char textureSampleGetFlipY(WK_TEXTURE_SAMPLE* textureSample);
WK_TEXTURE_PAGE* textureSampleGetTexturePage(WK_TEXTURE_SAMPLE* textureSample);
vector2f textureSampleGetUVLocation(WK_TEXTURE_SAMPLE* textureSample);
vector2f textureSampleGetUVSize(WK_TEXTURE_SAMPLE* textureSample);

#endif /* texture_sample_interface_h */
