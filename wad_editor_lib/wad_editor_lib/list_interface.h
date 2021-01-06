//
//  list_interface.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef list_interface_h
#define list_interface_h

#include "wad_editor_lib.h"

WE_LIST* listCreate(WE_LIST_DELEGATE* delegate);
void listRelease(WE_LIST* list);

void listSubscribe(WE_LIST* list, WE_LIST_SUBSCRIBER* subscriber);

WE_LIST_ITEM* listGetRootItem(WE_LIST* list);

void listClear(WE_LIST* list);

#endif /* list_interface_h */
