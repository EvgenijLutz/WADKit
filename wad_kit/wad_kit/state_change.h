//
//  state_change.h
//  wad_kit
//
//  Created by Евгений Лютц on 31.12.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef state_change_h
#define state_change_h

#include "private_interface.h"

typedef struct WK_DISPATCH
{
	unsigned short inRange;
	unsigned short outRange;
	unsigned short nextAnim;
	unsigned short frameIn;
}
WK_DISPATCH;

typedef struct WK_STATE_CHANGE
{
	WK_ANIMATION* animation;
	unsigned short stateId;
	/*!
	 @b WK_DISPATCH array.
	 */
	MAGIC_ARRAY dispatches;
}
WK_STATE_CHANGE;

#endif /* state_change_h */
