#ifndef _OPENAFE_INTERPRETER_
#define _OPENAFE_INTERPRETER_

#include "openAFE_Shared.h"

#include "Arduino.h"
#include <stdint.h>

/**
 * @brief Check if the passed command has a valid CRC of not.
 *
 * @param pCommand IN -- Command string to be checked, e.g.: "$CV,500,-500,250,10,2*10".
 * @return True if command is valid, false if not valid.
 */
bool openAFEInterpreter_isCommandCRCValid(String pCommand);

/**
 * @brief
 *
 * @param pCommandString IN -- Command string to be interpreted, e.g.: "$CVW,500,-500,250,10,2*47"
 * @param pCommandParams OUT -- Command parameters gotten from the command string.
 * @return >0 if successful, <0 on error (with error code).
 */
int openAFEInterpreter_getParametersFromCommand(String pCommandString, command_t *pCommandParams);

/**
 * @brief Gets the command ID of a given command string.
 *
 * @param pCommandString IN -- Command string to get the command code from, e.g.: "$CMD,RUN*81"
 * @return Command ID if successful, 0 (zero) on error.
 */
uint8_t _getCommandIdFromString(String pCommandString);

/**
 * @brief Populate the parameters in the command_t struct.
 * 
 * @note This function returns negative values on error, so don't check the success if different 
 * than zero.
 * 
 * @param pCommandId IN -- Command ID.
 * @param pCommandParams OUT -- Command parameters struct with the parameters set accordingly.
 * @param pCommandString IN -- Command message string.
 * @return >0 if successful, error code on error. 
 */
int _populateCommandParameters(uint8_t pCommandId, command_t *pCommandParams, String pCommandString);

/**
 * @brief Separate the parameters in a command, returning them in a string array. 
 * 
 * @param pParamArray OUT -- Array with the parameters (in String form). 
 * @param pCommandString IN -- The command message string to get the parameters from.
 * @param pSeparator IN -- REMOVE THIS ONE. 
 * @param pNumParams IN -- Number of parameters to get.
 */
void _separateCommandParameters(String pParamArray[], String pCommandString, char pSeparator, uint8_t pNumParams);

/**
 * @brief Parse Cyclic Voltammetry parameters.
 * 
 * @param pCommandParams OUT -- Command parameters struct.
 * @param pCommandString IN -- Command message string.
 */
void _parseCVParams(command_t *pCommandParams, String pCommandString);

/**
 * @brief Parse Differential Pulse Voltammetry parameters.
 * 
 * @param pCommandParams OUT -- Command parameters struct.
 * @param pCommandString IN -- Command message string.
 */
void _parseDPVParams(command_t *pCommandParams, String pCommandString);

/**
 * @brief Parse Square Wave Voltammetry parameters.
 * 
 * @param pCommandParams OUT -- Command parameters struct.
 * @param pCommandString IN -- Command message string.
 */
void _parseSWVParams(command_t *pCommandParams, String pCommandString);

/**
 * @brief Parse impedance spectroscopy parameters.
 * 
 * @param pCommandParams OUT -- Command parameters struct.
 * @param pCommandString IN -- Command message string.
 */
void _parseEISParams(command_t *pCommandParams, String pCommandString);

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

/**
 * @brief Get the interger checksum from a string/message.
 *
 * @param pString IN -- String with the CRC-8 to be stracted, e.g.: "$CV,500,-500,250,10,2*10"
 * @return CRC-8 integer (e.g. 16).
 */
uint8_t _getCRCIntegerFromString(String pString);

#endif//_OPENAFE_INTERPRETER_