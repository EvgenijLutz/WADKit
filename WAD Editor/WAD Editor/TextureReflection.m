//
//  TextureReflection.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "TextureReflection.h"

@implementation TextureReflection

- (instancetype)initWithTexturePage:(TEXTURE_PAGE*)texturePage texture:(id<MTLTexture>)texture
{
	self = [super init];
	if (self)
	{
		_texturePage = texturePage;
		_texture = texture;
	}
	return self;
}

@end
