#ifndef _OPENAFE_EXECUTIONER_
#define _OPENAFE_EXECUTIONER_

#include "openAFE_Shared.h"

/**
 * @brief Execute a received command with parameters.
 * 
 * @param pCommandParams IN -- Command struct with parameters.
 * @return >0 If successful, error code on error. 
 */
int openAFEExecutioner_executeCommand(command_t *pCommandParams);

#endif //_OPENAFE_EXECUTIONER_