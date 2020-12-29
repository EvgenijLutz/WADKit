//
//  execute_result.h
//  wad_kit
//
//  Created by Евгений Лютц on 05.12.2019.
//  Copyright © 2019 Eugene Lutz. All rights reserved.
//

#ifndef execute_result_h
#define execute_result_h

/**
 Result of function execution.
 */
typedef struct EXECUTE_RESULT
{
	int succeeded;
	char message[2048];
}
EXECUTE_RESULT;

/* Convenience methods */

static inline int executeResultIsSucceeded(EXECUTE_RESULT* executeResult)
{
	return executeResult ? executeResult->succeeded : 1;
}

static inline int executeResultIsFailed(EXECUTE_RESULT* executeResult)
{
	return executeResult ? !executeResult->succeeded : 0;
}

void executeResultSetSucceeded(EXECUTE_RESULT* executeResult);
void executeResultSetFailed(EXECUTE_RESULT* executeResult, const char* message);
void executeResultSetFailedCopy(EXECUTE_RESULT* executeResult, EXECUTE_RESULT* source);

#endif /* execute_result_h */
