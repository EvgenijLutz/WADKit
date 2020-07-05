//
//  TextureReflection.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;
#include "wad_editor_lib_link.h"

NS_ASSUME_NONNULL_BEGIN

@interface TextureReflection : NSObject

- (instancetype)initWithTexturePage:(TEXTURE_PAGE*)texturePage texture:(id<MTLTexture>)texture;

@property (readonly, nonatomic) TEXTURE_PAGE* texturePage;
@property (readonly, nonatomic) id<MTLTexture> texture;

@end

NS_ASSUME_NONNULL_END
