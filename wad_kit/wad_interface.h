//
//  wad_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef wad_interface_h
#define wad_interface_h

typedef struct EXECUTE_RESULT EXECUTE_RESULT;
typedef struct WAD WAD;
typedef struct STATIC STATIC;

WAD* wadCreate(void);
void wadRelease(WAD* wad);

WAD* wadLoadFromWadData(const unsigned char* data, long dataSize, EXECUTE_RESULT* executeResult);
unsigned char* wadGenerateWadData(WAD* wad, long* dataSize, EXECUTE_RESULT* executeResult);

unsigned int wadGetVersion(WAD* wad, EXECUTE_RESULT* executeResult);

unsigned int wadGetNumStatics(WAD* wad, EXECUTE_RESULT* executeResult);
STATIC* wadGetStaticByIndex(WAD* wad, unsigned int staticIndex, EXECUTE_RESULT* executeResult);
STATIC* wadGetStaticById(WAD* wad, unsigned int staticId, EXECUTE_RESULT* executeResult);

#endif /* wad_interface_h */
