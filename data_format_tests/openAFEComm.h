#ifndef _OPENAFE_COMM_
#define _OPENAFE_COMM_

#include "openafe.h"
#include "Arduino.h"
#include "stdint.h"

/**
 * @brief Await OpenAFE command strings and execute them. Call this function to 
 * continuously process commands for the OpenAFE.
 * 
 * @warning THIS IS A BLOCKING FUNCTION! Any function after this one will not run.
 */
void openAFEComm_waitForCommands(AFE *pOpenafeInstance);

#endif //_OPENAFE_COMM_