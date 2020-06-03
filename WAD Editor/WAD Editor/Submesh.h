//
//  Submesh.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;

NS_ASSUME_NONNULL_BEGIN

@interface Submesh : NSObject

- (instancetype)initWithIndexBuffer:(id<MTLBuffer>)indexBuffer numIndices:(unsigned int)numIndices texture:(id<MTLTexture>)texture;

@property (readonly) id<MTLBuffer> indexBuffer;
@property (readonly) unsigned int numIndices;
@property (readonly) id<MTLTexture> texture;

@end

NS_ASSUME_NONNULL_END
