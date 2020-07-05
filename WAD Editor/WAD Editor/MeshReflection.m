//
//  MeshReflection.m
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#import "MeshReflection.h"

@implementation MeshReflection
{
	NSArray<Submesh*>* submeshes;
}

- (instancetype)initWithMesh:(MESH*)mesh vertexBuffer:(id<MTLBuffer>)vertexBuffer submeshes:(NSArray<Submesh*>*)meshSubmeshes shaded:(BOOL)shaded
{
	self = [super init];
	if (self)
	{
		_mesh = mesh;
		submeshes = [meshSubmeshes copy];
		_vertexBuffer = vertexBuffer;
		_shaded = shaded;
	}
	return self;
}


- (unsigned int)numSubmeshes
{
	return (unsigned int)submeshes.count;
}

- (Submesh*)submeshAtIndex:(unsigned int)submeshIndex
{
	return submeshes[submeshIndex];
}

@end
