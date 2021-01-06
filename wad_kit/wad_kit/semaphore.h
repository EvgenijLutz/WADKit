//
//  semaphore.h
//  wad_kit
//
//  Created by Евгений Лютц on 06.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef semaphore_h
#define semaphore_h

typedef struct WK_SEMAPHORE
{
	WK_SYSTEM* system;
	void* semaphoreId;
}
WK_SEMAPHORE;

#endif /* semaphore_h */
