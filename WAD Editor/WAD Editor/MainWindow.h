//
//  MainWindow.h
//  WAD Editor
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

@import Cocoa;
@import Metal;
#import "WKEditorDelegate.h"

NS_ASSUME_NONNULL_BEGIN

@interface MainWindow : NSWindow<NSOutlineViewDataSource, NSOutlineViewDelegate, WKEditorDelegate>

- (void)initializeWithMetalDevice:(id<MTLDevice>)metalDevice;

@end

NS_ASSUME_NONNULL_END
