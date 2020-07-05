//
//  WadItemsDataSource.h
//  WAD Editor
//
//  Created by Евгений Лютц on 13.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import AppKit;
#include "wad_editor_lib_link.h"

NS_ASSUME_NONNULL_BEGIN

@interface WadItemsDataSource : NSObject<NSOutlineViewDataSource, NSOutlineViewDelegate>

- (instancetype)initWithEditor:(WAD_EDITOR*)wadEditor;

@end

NS_ASSUME_NONNULL_END
