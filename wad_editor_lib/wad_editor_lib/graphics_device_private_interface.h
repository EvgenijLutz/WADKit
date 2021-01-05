//
//  graphics_device_private_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 04.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef graphics_device_private_interface_h
#define graphics_device_private_interface_h

#include "private_interface.h"

void graphicsDeviceReleaseCommandQueue(GRAPHICS_DEVICE* device, COMMAND_QUEUE* commandQueue);

#endif /* graphics_device_private_interface_h */
