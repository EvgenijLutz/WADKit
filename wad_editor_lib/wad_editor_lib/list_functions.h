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
//typedef void (* WE_LIST_WILL_BE_CLEARED_FUNC)(WE_LIST* list, void* userInfo);
typedef struct WE_LIST_DELEGATE
{
	WE_LIST_ITEM_ADDED_FUNC itemAddedFunc;
	WE_LIST_ITEM_WILL_BE_REMOVED_FUNC itemWillBeRemovedFunc;
	void* userInfo;
}
WE_LIST_DELEGATE;


typedef void (* WE_LIST_ITEM_SELECTED_FUNC)(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo);
typedef void (* WE_LIST_ITEM_DESELECTED_FUNC)(WE_LIST* list, WE_LIST_ITEM* item, void* userInfo);
typedef struct WE_LIST_SUBSCRIBER
{
	WE_LIST_ITEM_SELECTED_FUNC itemSelectedFunc;
	WE_LIST_ITEM_DESELECTED_FUNC itemDeselectedFunc;
	void* userInfo;
}
WE_LIST_SUBSCRIBER;

#endif /* list_functions_h */
