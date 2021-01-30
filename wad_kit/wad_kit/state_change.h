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


typedef struct DISPATCH
{
	unsigned short inRange;
	unsigned short outRange;
	unsigned short nextAnim;
	unsigned short frameIn;
}
DISPATCH;

typedef struct STATE_CHANGE
{
	ANIMATION* animation;
	unsigned short stateId;
	/*!
	 @b DISPATCH array.
	 */
	MAGIC_ARRAY dispatches;
}
STATE_CHANGE;

#endif /* state_change_h */
