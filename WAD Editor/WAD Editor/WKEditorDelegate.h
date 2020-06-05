//
//  WKEditorDelegate.h
//  WAD Editor
//
//  Created by Евгений Лютц on 05.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
#include "wad_kit_link.h"

@class WKEditor;

NS_ASSUME_NONNULL_BEGIN

@protocol WKEditorDelegate <NSObject>

- (void)editor:(WKEditor*)editor wadLoaded:(WAD*)wad;

@end

NS_ASSUME_NONNULL_END
