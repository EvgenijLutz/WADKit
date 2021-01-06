//
//  list.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef list_h
#define list_h

#include "private_interface.h"

typedef struct WE_LIST
{
	WE_LIST_DELEGATE delegate;
	WE_LIST_SUBSCRIBER subscriber;
	int hasSubscriber;
	
	DATA_ALLOCATOR* itemAllocator;
	WE_LIST_ITEM rootItem;
}
WE_LIST;

#endif /* list_h */
