//
//  SubmeshGenerator.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;
#include "wad_editor_lib_link.h"

@class ResourceStorage;
@class Submesh;

NS_ASSUME_NONNULL_BEGIN

@interface SubmeshGenerator : NSObject

- (instancetype)initWithStorage:(ResourceStorage*)resourceStorage wad:(WAD*)_wad;

- (void)addPolygon:(POLYGON*)polygon;
- (NSArray<Submesh*>*)generateSubmeshesWithBlitEncoder:(id<MTLBlitCommandEncoder>)blitCommandEncoder;

@end

NS_ASSUME_NONNULL_END
