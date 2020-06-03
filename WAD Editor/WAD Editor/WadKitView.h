//
//  WadKitView.h
//  WAD Editor
//
//  Created by Евгений Лютц on 26.05.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import MetalKit;

@class WKRenderer;

NS_ASSUME_NONNULL_BEGIN

@interface WadKitView : MTKView

@property (readonly) WKRenderer* renderer;

@end

NS_ASSUME_NONNULL_END
