//
//  Submesh.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "Submesh.h"

@implementation Submesh

- (instancetype)initWithIndexBuffer:(id<MTLBuffer>)indexBuffer numIndices:(unsigned int)numIndices texture:(id<MTLTexture>)texture
{
	self = [super init];
	if (self)
	{
		_indexBuffer = indexBuffer;
		_numIndices = numIndices;
		_texture = texture;
	}
	return self;
}

@end
