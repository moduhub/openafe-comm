#ifndef _OPENAFE_INTERPRETER_
#define _OPENAFE_INTERPRETER_

#include "openAFE_Shared.h"

#include "Arduino.h"
#include <stdint.h>

/**
 * @brief
 *
 * @param pCommandString IN -- Command string to be interpreted, e.g.: "$CVW,500,-500,250,10,2*47"
 * @param pCommandParams OUT -- Command parameters gotten from the command string.
 * @return >0 if successful, <0 on error (with error code).
 */
int openAFEInterpreter_getParametersFromCommand(String pCommandString, command_t *pCommandParams);

#endif//_OPENAFE_INTERPRETER_