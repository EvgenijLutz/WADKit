//
//  WadKitViewDelegate.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import MetalKit;
#include <simd/simd.h>

#include "wad_editor_lib_link.h"

@protocol WKViewport;

NS_ASSUME_NONNULL_BEGIN

@interface WadKitViewDelegate : NSObject<MTKViewDelegate>

@end

NS_ASSUME_NONNULL_END
