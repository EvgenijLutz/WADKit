//
//  WKEditor.h
//  WAD Editor
//
//  Created by Евгений Лютц on 02.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

@import Foundation;
@import Metal;
#import "WKViewport.h"
#import "WKEditorDelegate.h"

NS_ASSUME_NONNULL_BEGIN

@interface WKEditor : NSObject<WKViewport>

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice;

- (void)selectNextObject;
- (void)selectPreviousObject;

- (void)loadWadByPath:(NSString*)wadPath;
- (WAD*)wad;

- (void)selectMeshAtIndex:(unsigned int)meshIndex;

@property (nonatomic) id<WKEditorDelegate> delegate;

@end

NS_ASSUME_NONNULL_END
