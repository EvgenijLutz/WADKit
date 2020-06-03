//
//  WKViewport.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
#include <simd/simd.h>

@class WKRenderer;

NS_ASSUME_NONNULL_BEGIN

@protocol WKViewport <NSObject>

@required
- (void)sizeChanged:(simd_float2)viewportSize;
- (void)drawWithRenderer:(WKRenderer*)renderer;

@end

NS_ASSUME_NONNULL_END
