#ifndef _OPENAFE_COMM_
#define _OPENAFE_COMM_

#include "Arduino.h"
#include "stdint.h"
#include <stdio.h>

#include "openafe.h"

#include "openAFEComm_Shared.hpp"
#include "executioner/openAFE_Executioner.hpp"
#include "interpreter/openAFE_Interpreter.hpp"
#include "serial/openAFE_Serial.hpp"

#include "util/CRC.hpp"

/**
 * @brief Await OpenAFE command strings and execute them. Call this function to 
 * continuously process commands for the OpenAFE.
 * 
 * @warning THIS IS A BLOCKING FUNCTION! Any function after this one will not run.
 */
void openAFEComm_waitForCommands(void);

#endif //_OPENAFE_COMM_