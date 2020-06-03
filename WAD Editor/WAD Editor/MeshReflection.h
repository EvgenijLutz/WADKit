//
//  MeshReflection.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;

@class Submesh;

NS_ASSUME_NONNULL_BEGIN

@interface MeshReflection : NSObject

- (instancetype)initWithVertexBuffer:(id<MTLBuffer>)vertexBuffer submeshes:(NSArray<Submesh*>*)meshSubmeshes shaded:(BOOL)shaded;

- (unsigned int)numSubmeshes;
- (Submesh*)submeshAtIndex:(unsigned int)submeshIndex;

@property (readonly) BOOL shaded;
@property (readonly) id<MTLBuffer> vertexBuffer;

@end

NS_ASSUME_NONNULL_END
