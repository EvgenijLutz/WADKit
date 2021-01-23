//
//  GraphicsMesh_apple.m
//  graphics
//
//  Created by Евгений Лютц on 23.01.21.
//

#import "GraphicsMesh_apple.h"

@implementation GraphicsMesh

- (instancetype)initWithVertexBuffer:(id<MTLBuffer>)vertexBuffer texture:(id<MTLTexture>)texture
{
	self = [super init];
	if (self)
	{
		_vertexBuffer = vertexBuffer;
		_texture = texture;
	}
	return self;
}

@end
