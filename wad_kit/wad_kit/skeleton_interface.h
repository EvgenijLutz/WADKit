//
//  skeleton_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_skeleton_interface_h
#define wad_kit_skeleton_interface_h

#include "wad_kit.h"

unsigned int skeletonGetNumJoints(SKELETON* skeleton);
JOINT* skeletonGetJoint(SKELETON* skeleton, unsigned int jointIndex);

#endif /* wad_kit_skeleton_interface_h */
