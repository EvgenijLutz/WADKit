//
//  private_common.h
//  wad_kit
//
//  Created by Евгений Лютц on 03.01.21.
//  Copyright © 2021 Eugene Lutz. All rights reserved.
//

#ifndef private_common_h
#define private_common_h

// MARK: - System headers

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// MARK: - Macros

#if DEBUG

#define debug_memset(data, value, size) memset((data), (value), (size))

#else

#define debug_memset(data, value, size) /* do nothing */

#endif

#endif /* private_common_h */
