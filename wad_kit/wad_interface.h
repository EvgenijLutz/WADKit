//
//  wad_interface.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef wad_interface_h
#define wad_interface_h

typedef struct WAD WAD;
typedef struct EXECUTE_RESULT EXECUTE_RESULT;

WAD* wadCreate(void);
void wadRelease(WAD* wad);
WAD* wadLoadFromWadData(const unsigned char* data, long dataSize, EXECUTE_RESULT* executeResult);	// wadLoadFromWADData?
unsigned char* wadGenerateWadData(WAD* wad, long* dataSize, EXECUTE_RESULT* executeResult);		// wadGenerateWADData?

#endif /* wad_interface_h */
