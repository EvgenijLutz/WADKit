//
//  texture_page_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.07.20.
//  Copyright © 2020 Eugene Lutz. All rights reserved.
//

#ifndef texture_page_interface_h
#define texture_page_interface_h

typedef struct TEXTURE_PAGE TEXTURE_PAGE;

unsigned char* texturePageGetData(TEXTURE_PAGE* texturePage);

#endif /* texture_page_interface_h */
