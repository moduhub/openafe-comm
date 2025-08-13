#include "openAFE_Interpreter.h"

#include <string.h>
#include <stdio.h>

#include "openAFE_Error_Codes.h"


// ==== PUBLIC ==== //

bool openAFEInterpreter_isCommandCRCValid(String pCommand){
	uint8_t tCommandLength = pCommand.length();

	char tCommandArray[tCommandLength + 1];

	pCommand.toCharArray(tCommandArray, tCommandLength + 1);

	uint8_t tCalculatedChecksum = 0;

	// the iterator starts at 1 to eliminate the suffix, '$'
	// the '-3' is to remove the asterix and the CRC, "*7C"
	for (uint8_t i = 1; i < tCommandLength - 3; i++)
		tCalculatedChecksum ^= tCommandArray[i];

	uint8_t tChecksumFromCommand = _getCRCIntegerFromString(pCommand);

	bool isCommandValid = (tCalculatedChecksum - tChecksumFromCommand) == 0 ? true : false;

	return isCommandValid;
}

int openAFEInterpreter_getParametersFromCommand(String pCommandString, command_t *pCommandParams){
	int interpreterResult = 0;

	memset(pCommandParams, 0x0, sizeof(command_t));

	uint8_t tCommandId = _getCommandIdFromString(pCommandString);

	pCommandParams->id = tCommandId;

	if (!tCommandId)
		return ERROR_INVALID_COMMAND;
  
	interpreterResult = _populateCommandParameters(tCommandId, pCommandParams, pCommandString);

	return interpreterResult;
}

// ====        ==== //



// ==== PRIVATE ==== //

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
  else if (strcmp(tFirstCommandCode, "EIS") == 0)
	{
		return CMDID_EIS;
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
		else if (strcmp(tSecondCommandCode, "CUR") == 0)
		{
			return CMDID_CUR;
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

int _populateCommandParameters(uint8_t pCommandId, command_t *pCommandParams, String pCommandString){
	int returnValue = 1;

	switch (pCommandId)
	{
	case CMDID_CHK:
		pCommandParams->id = CMDID_CHK;
		break;

	case CMDID_DIE:
		pCommandParams->id = CMDID_DIE;
		break;

	case CMDID_TIA:
		pCommandParams->id = CMDID_TIA;
		_parseTIAParams(pCommandParams, pCommandString);
		break;

	case CMDID_CUR:
		_parseCurrentRangeParams(pCommandParams, pCommandString);
		break;

	case CMDID_CVW:
		_parseCVParams(pCommandParams, pCommandString);
		break;
	
	case CMDID_DPV:
		_parseDPVParams(pCommandParams, pCommandString);
		break;

	case CMDID_SWV:
		_parseSWVParams(pCommandParams, pCommandString);
		break;
  
  case CMDID_EIS:
		_parseEISParams(pCommandParams, pCommandString);
		break;

	default:
		returnValue = ERROR_INVALID_COMMAND;
		break;
	}

	return returnValue;
}

void _parseCVParams(command_t *pCommandParams, String pCommandString){

	String tParamArray[6];

	_separateCommandParameters(tParamArray, pCommandString, ',', 6);

	pCommandParams->id = CMDID_CVW;
	pCommandParams->settlingTime = tParamArray[0].toInt();
	pCommandParams->startingPotential = tParamArray[1].toFloat();
	pCommandParams->endingPotential = tParamArray[2].toFloat();
	pCommandParams->scanRate = tParamArray[3].toFloat();
	pCommandParams->stepSize = tParamArray[4].toFloat();
	pCommandParams->numCycles = tParamArray[5].toInt();

	return;
}

void _parseDPVParams(command_t *pCommandParams, String pCommandString){

  //Serial.print("Recebido para DPV: ");
  //Serial.println(pCommandString);

  String tParamArray[9];

  _separateCommandParameters(tParamArray, pCommandString, ',', 9);

  pCommandParams->id = CMDID_DPV;
  pCommandParams->settlingTime = tParamArray[0].toInt();
  pCommandParams->startingPotential = tParamArray[1].toFloat();
  pCommandParams->endingPotential = tParamArray[2].toFloat();
  pCommandParams->pulseAmplitude = tParamArray[3].toFloat();
  pCommandParams->stepSize = tParamArray[4].toFloat();
  pCommandParams->pulseWidth = tParamArray[5].toInt();
  pCommandParams->baseWidth = tParamArray[6].toInt();
  pCommandParams->samplePeriodPulse = tParamArray[7].toInt();
  pCommandParams->samplePeriodBase = tParamArray[8].toInt();

}

void _parseSWVParams(command_t *pCommandParams, String pCommandString){
	String tParamArray[7];

	_separateCommandParameters(tParamArray, pCommandString, ',', 7);

	pCommandParams->id = CMDID_SWV;
	pCommandParams->settlingTime = tParamArray[0].toInt();
	pCommandParams->startingPotential = tParamArray[1].toFloat();
	pCommandParams->endingPotential = tParamArray[2].toFloat();
	pCommandParams->scanRate = tParamArray[3].toFloat();
	pCommandParams->pulseAmplitude = tParamArray[4].toFloat();
	pCommandParams->pulseFrequency = tParamArray[5].toFloat();
	pCommandParams->samplePeriodPulse = tParamArray[6].toInt();

	return;
}

void _parseEISParams(command_t *pCommandParams, String pCommandString){
	String tParamArray[5];

	_separateCommandParameters(tParamArray, pCommandString, ',', 5);

	pCommandParams->id = CMDID_EIS;
	pCommandParams->settlingTime = tParamArray[0].toInt();
	pCommandParams->startingOmega = tParamArray[1].toInt();
	pCommandParams->endingOmega = tParamArray[2].toInt();
	pCommandParams->stepForADecade = tParamArray[3].toInt();
	pCommandParams->scanRate = tParamArray[4].toFloat();

	return;
}

void _parseTIAParams(command_t *pCommandParams, String pCommandString){
	String tParamsArray[2];
	
	_separateCommandParameters(tParamsArray, pCommandString, ',', 2);

	pCommandParams->id = CMDID_TIA;
	pCommandParams->gainTIA = tParamsArray[1].toInt();

	return;
}

void _parseCurrentRangeParams(command_t *pCommandParams, String pCommandString){
	String tParamsArray[2];

	_separateCommandParameters(tParamsArray, pCommandString, ',', 2);
	pCommandParams->id = CMDID_CUR;
	pCommandParams->currentRange = tParamsArray[1].toInt();

	return;
}

void _separateCommandParameters(String pParamArray[], String pCommandString, char pSeparator, uint8_t pNumParams) {
  // Debug
  //Serial.print(F("Comando recebido: "));
  //Serial.println(pCommandString);

  // Find first comma and asterisk
  int startPos = pCommandString.indexOf(',');
  int endPos = pCommandString.indexOf('*');
  
  if (startPos == -1 || endPos == -1) {
    Serial.println(F("Error: invalid command format"));
    return;
  }

  // Extract parameters section
  String paramsStr = pCommandString.substring(startPos + 1, endPos);
  
  // Debug
  //Serial.print(F("Parametros: "));
  //Serial.println(paramsStr);

  // Clear parameter array
  for (uint8_t i = 0; i < pNumParams; i++) {
    pParamArray[i] = "";
  }

  // Parse parameters manually without using multiple indexOf calls
  uint8_t paramIndex = 0;
  int strLen = paramsStr.length();
  int lastComma = 0;
  
  // Handle each parameter
  for (int i = 0; i < strLen && paramIndex < pNumParams; i++) {
    if (paramsStr[i] == ',' || i == strLen - 1) {
      // Extract parameter
      if (i == strLen - 1 && paramsStr[i] != ',') {
        // Include last character if at end of string
        pParamArray[paramIndex] = paramsStr.substring(lastComma, i + 1);
      } else {
        pParamArray[paramIndex] = paramsStr.substring(lastComma, i);
      }
      
      lastComma = i + 1;
      paramIndex++;
    }
  }

  // Verify we got all parameters
  if (paramIndex < pNumParams) {
    Serial.println(F("Warning: fewer parameters than expected"));
  }
}

uint8_t _getCRCIntegerFromString(String pString) {
	uint8_t tStringLength = pString.length();

	char tStringCheckSum[] = {pString[tStringLength - 2], pString[tStringLength - 1]};
	uint8_t tIntegerChecksum;

	tIntegerChecksum = tStringCheckSum[1] > 0x3a ? (uint8_t)(tStringCheckSum[1] - 0x37) : (uint8_t)(tStringCheckSum[1] - 0x30);
	tIntegerChecksum += tStringCheckSum[0] << 4;

	return tIntegerChecksum;
}

// ====         ==== //