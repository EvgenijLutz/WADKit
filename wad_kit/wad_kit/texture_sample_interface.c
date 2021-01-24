//
//  texture_sample_interface.c
//  wad_kit
//
//  Created by Евгений Лютц on 29.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#include "private_interface.h"

unsigned char textureSampleGetX(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->x;
}

unsigned char textureSampleGetY(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->y;
}

unsigned char textureSampleGetAddX(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->addW;
}

unsigned char textureSampleGetAddY(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->addH;
}

char textureSampleGetFlipX(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->flipX;
}

char textureSampleGetFlipY(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->flipY;
}


TEXTURE_PAGE* textureSampleGetTexturePage(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->page;
}

vector2f textureSampleGetUVLocation(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->uvLocation;
}

vector2f textureSampleGetUVSize(TEXTURE_SAMPLE* textureSample)
{
	assert(textureSample);
	return textureSample->uvSize;
}
