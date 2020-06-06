//
//  joint.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef wad_kit_joint_h
#define wad_kit_joint_h

typedef struct JOINT
{
	/**
	 Documentation:
	 The opCode takes the values 0, 1, 2, 3, where:
	 0 = stack not used. Link the current mesh to the previous mesh.
	 1 = pull the parent from the stack. Link the current mesh to the parent.
	 2 = push the parent into the stack. Link the current mesh to the parent.
	 3 = read the parent in the stack. Link the current mesh to the parent.
	 */
	int opCode;
	
	int dx;
	int dy;
	int dz;
}
JOINT;

#endif /* wad_kit_joint_h */
