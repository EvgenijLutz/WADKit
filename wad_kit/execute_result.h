//
//  execute_result.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef execute_result_h
#define execute_result_h

typedef struct EXECUTE_RESULT
{
	int succeeded;
	char message[2048];
}
EXECUTE_RESULT;

void executeResultSucceeded(EXECUTE_RESULT* executeResult);
void executeResultFailed(EXECUTE_RESULT* executeResult, const char* message);
void executeResultFailedCopy(EXECUTE_RESULT* executeResult, EXECUTE_RESULT* source);

#endif /* execute_result_h */
