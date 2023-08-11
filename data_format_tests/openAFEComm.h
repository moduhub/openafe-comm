#ifndef _OPENAFE_COMM_
#define _OPENAFE_COMM_

#include "openafe.h"
#include "Arduino.h"
#include "stdint.h"

/**
 * @brief Blocking functions that waits for commands 
 * coming from the App and process it.
 *
 */
void openAFEComm_waitForCommands(AFE *pOpenafeInstance);

#endif //_OPENAFE_COMM_