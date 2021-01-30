//
//  AppDelegate.h
//  WAD Editor
//
//  Created by Евгений Лютц on 16.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

@import Cocoa;
#include "wad_editor_lib_link.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

+ (SYSTEM*)system;
+ (GraphicsDevice*)graphicsDevice;

@end

