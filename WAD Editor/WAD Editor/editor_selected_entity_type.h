//
//  editor_selected_entity_type.h
//  WAD Editor
//
//  Created by Евгений Лютц on 06.06.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef editor_selected_entity_type_h
#define editor_selected_entity_type_h

typedef enum SELECTED_ENTITY_TYPE
{
	SELECTED_ENTITY_TYPE_NONE = 0,
	//SELECTED_ENTITY_TYPE_TEXTURE_PAGE,
	SELECTED_ENTITY_TYPE_MESH,
	//SELECTED_ENTITY_TYPE_SKELETON,
	//SELECTED_ENTITY_TYPE_MOVABLE,
	SELECTED_ENTITY_TYPE_STATIC
}
SELECTED_ENTITY_TYPE;

#endif /* editor_selected_entity_type_h */
