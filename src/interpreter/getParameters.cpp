#include "openAFE_Interpreter.hpp"

#include "Arduino.h"

int openAFEInterpreter_getParametersFromCommand(const String& pCommandString, command_t *pCommandParams){
	memset(pCommandParams, 0x0, sizeof(command_t));

	uint8_t tCommandId = commandToId(pCommandString);

	pCommandParams->id = tCommandId;
	if (!tCommandId) return ERROR_INVALID_COMMAND;

	switch (tCommandId) {
    case CMDID_CHK:
    case CMDID_DIE:
      return 1; 

    case CMDID_TIA:
      _parseTIAParams(pCommandParams, pCommandString);
      return 1;

    case CMDID_CUR:
      _parseCurrentRangeParams(pCommandParams, pCommandString);
      return 1;

    case CMDID_CVW:
      _parseCVParams(pCommandParams, pCommandString);
      return 1;

    case CMDID_DPV:
      _parseDPVParams(pCommandParams, pCommandString);
      return 1;

    case CMDID_SWV:
      _parseSWVParams(pCommandParams, pCommandString);
      return 1;

    case CMDID_EIS:
      _parseEISParams(pCommandParams, pCommandString);
      return 1;

    default:
      return ERROR_INVALID_COMMAND;
  }
}