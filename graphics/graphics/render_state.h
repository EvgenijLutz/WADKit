//
//  render_state.h
//  graphics
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef render_state_h
#define render_state_h

#include "private_interface.h"

typedef struct RENDER_STATE
{
	GRAPHICS_DEVICE* device;
	void* userInfo;
}
RENDER_STATE;

#endif /* render_state_h */
