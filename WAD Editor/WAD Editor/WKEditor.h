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

NS_ASSUME_NONNULL_BEGIN

@interface WKEditor : NSObject<WKViewport>

- (instancetype)initWithMetalDevice:(id<MTLDevice>)metalDevice;

- (void)selectNextObject;
- (void)selectPreviousObject;

- (void)loadWadByPath:(NSString*)wadPath;

@end

NS_ASSUME_NONNULL_END
