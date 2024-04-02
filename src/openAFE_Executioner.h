#ifndef _OPENAFE_EXECUTIONER_
#define _OPENAFE_EXECUTIONER_

#include "openafe.h"
#include "openAFE_Shared.h"

/**
 * @brief Set the callback to pass the point result up. The callback function should be of void 
 * type and receive two float parameters, one being the voltage in millivolts (mV) and the other 
 * the current in microAmps (uA), e.g.: void handlePoint(float pVoltage_mV, float pVoltage_uA).
 *
 * @param pPointResultMessageCallback IN -- Callback to the function to send the SGL point message.
 */
void openAFEExecutioner_setPointResultMessageCallback(void (*pPointResultMessageCallback)(float, float));

/**
 * @brief Execute a received command with parameters.
 * 
 * @param pCommandParams IN -- Command struct with parameters.
 * @return >0 If successful, error code on error. 
 */
int openAFEExecutioner_executeCommand(AFE *pOpenafeInstance, command_t *pCommandParams);

#endif //_OPENAFE_EXECUTIONER_