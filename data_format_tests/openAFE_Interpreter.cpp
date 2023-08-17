#include "openAFE_Interpreter.h"

#include <string.h>
#include <stdio.h>

#include "openAFE_Error_Codes.h"

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

int openAFEInterpreter_getParametersFromCommand(String pCommandString, command_t *pCommandParams)
{
	int interpreterResult = 0;

	memset(pCommandParams, 0x0, sizeof(command_t));

	uint8_t tCommandId = _getCommandIdFromString(pCommandString);

	pCommandParams->id = tCommandId;

	if (!tCommandId)
		return ERROR_INVALID_COMMAND;

	interpreterResult = _populateCommandParameters(tCommandId, pCommandParams, pCommandString);

	return interpreterResult;
}

int _populateCommandParameters(uint8_t pCommandId, command_t *pCommandParams, String pCommandString)
{
	int returnValue = 1;

	switch (pCommandId)
	{
	case CMDID_CHK:
		pCommandParams->id = CMDID_CHK;
		break;

	case CMDID_DIE:
		pCommandParams->id = CMDID_DIE;
		break;

	case CMDID_CVW:
		_parseCVParams(pCommandParams, pCommandString);
		break;
	
	default:
		returnValue = ERROR_INVALID_COMMAND;
		break;
	}

	return returnValue;
}

void _parseCVParams(command_t *pCommandParams, String pCommandString)
{
	String tParamArray[5];

	_separateCommandParameters(tParamArray, pCommandString, ',', 5);

	pCommandParams->id = CMDID_CVW;
	pCommandParams->endingPotential = tParamArray[0].toFloat();
	pCommandParams->startingPotential = tParamArray[1].toFloat();
	pCommandParams->scanRate = tParamArray[2].toFloat();
	pCommandParams->stepSize = tParamArray[3].toFloat();
	pCommandParams->numCycles = tParamArray[4].toInt();

	return;
}

void _separateCommandParameters(String pParamArray[], String pCommandString, char pSeparator, uint8_t pNumParams)
{
	char charData[pCommandString.length() + 1];
	pCommandString.toCharArray(charData, sizeof(charData));

	char *token = strtok(charData, &pSeparator);
	token = strtok(NULL, &pSeparator); // ditches the suffix and the command code "$CMD"
	int i = 0;

	while (token != NULL && i < pNumParams)
	{
		pParamArray[i] = String(token);
		token = strtok(NULL, &pSeparator);
		i++;
	}
}

uint8_t _getCommandIdFromString(String pCommandString)
{
	char tFirstCommandCode[4];

	// Gets the first command code from the message
	// Example: $CMD,RUN*81 -> gets CMD
	char tCommandArray[pCommandString.length() + 1];

	pCommandString.toCharArray(tCommandArray, pCommandString.length());
	
	tCommandArray[pCommandString.length()] = '\0';

	memcpy(tFirstCommandCode, tCommandArray + 1, 3);
	tFirstCommandCode[3] = '\0';

	if (strcmp(tFirstCommandCode, "CVW") == 0)
	{
		return CMDID_CVW;
	}
	else if (strcmp(tFirstCommandCode, "DPV") == 0)
	{
		return CMDID_DPV;
	}
	else if (strcmp(tFirstCommandCode, "SWV") == 0)
	{
		return CMDID_SWV;
	}
	else if (strcmp(tFirstCommandCode, "CMD") == 0)
	{
		char tSecondCommandCode[4];

		// Gets the second command from message:
		// Example: $CMD,RUN*81 -> gets the RUN
		memcpy(tSecondCommandCode, tCommandArray + 5, 3);
		tSecondCommandCode[3] = '\0';

		if (strcmp(tSecondCommandCode, "RUN") == 0)
		{
			return CMDID_RUN;
		}
		else if (strcmp(tSecondCommandCode, "CHK") == 0)
		{
			return CMDID_CHK;
		}
		else if (strcmp(tSecondCommandCode, "DIE") == 0)
		{
			return CMDID_DIE;
		}
		else if (strcmp(tSecondCommandCode, "TIA") == 0)
		{
			return CMDID_TIA;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}