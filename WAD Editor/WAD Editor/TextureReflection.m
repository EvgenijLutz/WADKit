//
//  TextureReflection.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "TextureReflection.h"

@implementation TextureReflection

- (instancetype)init
{
	self = [super init];
	if (self)
	{
		_texture = nil;
		_index = 0;
	}
	return self;
}

@end
