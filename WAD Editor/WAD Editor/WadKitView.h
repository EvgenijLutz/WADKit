//
//  WadKitView.h
//  WAD Editor
//
//  Created by Евгений Лютц on 26.05.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import MetalKit;
#include "wad_editor_lib_link.h"

@class ResourceStorage;
@class WKRenderer;

NS_ASSUME_NONNULL_BEGIN

@interface WadKitView : MTKView

- (instancetype)initWithFrame:(NSRect)frame resourceStorage:(ResourceStorage*)resourceStorage;

@property (nonatomic) WAD_EDITOR_VIEWPORT* viewport;
@property (nonatomic) ResourceStorage* resourceStorage;

@property (readonly) WKRenderer* renderer;

@end

NS_ASSUME_NONNULL_END
