#ifndef _OPENAFE_INTERPRETER_
#define _OPENAFE_INTERPRETER_

#include "Arduino.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "../openAFEComm_Shared.hpp"

/**
 * @brief
 *
 * @param pCommandString IN -- Command string to be interpreted, e.g.: "$CVW,500,-500,250,10,2*47"
 * @param pCommandParams OUT -- Command parameters gotten from the command string.
 * @return >0 if successful, <0 on error (with error code).
 */
int openAFEInterpreter_getParametersFromCommand(const String& pCommandString, command_t *pCommandParams);

/**
 * @brief Gets the command ID of a given command string.
 *
 * @param pCommandString IN -- Command string to get the command code from, e.g.: "$CMD,RUN*81"
 * @return Command ID if successful, 0 (zero) on error.
 */
uint8_t commandToId(const String& pCommandString);

/**
 * @brief Separate the parameters in a command, returning them in a string array. 
 * 
 * @param pParamArray OUT -- Array with the parameters (in String form). 
 * @param pCommandString IN -- The command message string to get the parameters from.
 * @param pSeparator IN -- REMOVE THIS ONE. 
 * @param pNumParams IN -- Number of parameters to get.
 */
void separateCommandParameters(String pParamArray[], String pCommandString, char pSeparator, uint8_t pNumParams);

/**
 * @brief Parse Voltammetry and EIS parameters.
 * 
 * @param pCommandParams OUT -- Command parameters struct.
 * @param pCommandString IN -- Command message string.
 */
void _parseParams(command_t *p, String cmd, uint8_t cmdId);

/**
 * @brief Parse TIA Gain command parameters.
 * 
 * @param pCommandParams OUT -- Command parameters struct.
 * @param pCommandString IN -- Command message string.
 */
void _parseTIAParams(command_t *pCommandParams, String pCommandString);

/**
 * @brief Parse Current range command parameters.
 * 
 * @param pCommandParams OUT -- Command parameters struct.
 * @param pCommandString IN -- Command message string.
 */
void _parseCurrentRangeParams(command_t *pCommandParams, String pCommandString);

#endif//_OPENAFE_INTERPRETER_