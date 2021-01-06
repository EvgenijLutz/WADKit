//
//  list_functions.h
//  wad_editor_lib
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef list_functions_h
#define list_functions_h

#include "wad_editor_lib.h"

typedef void (* WE_LIST_ITEM_ADDED_FUNC)(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo);
typedef void (* WE_LIST_ITEM_WILL_BE_REMOVED_FUNC)(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo);
typedef void (* WE_LIST_ITEM_WILL_BE_CLEARED_FUNC)(WE_LIST* list, void* userInfo);

#endif /* list_functions_h */
