//
//  GraphicsView.h
//  graphics
//
//  Created by Евгений Лютц on 05.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

@import MetalKit;
#include "graphics_apple.h"

NS_ASSUME_NONNULL_BEGIN

@interface GraphicsView : MTKView<MTKViewDelegate>

- (instancetype)initWithFrame:(NSRect)frame graphicsDevice:(GRAPHICS_DEVICE*)graphicsDevice;

@property (nonatomic, readonly) GRAPHICS_DEVICE* graphicsDevice;
@property (nonatomic, readonly) GRAPHICS_VIEW* graphicsView;

@end

NS_ASSUME_NONNULL_END
