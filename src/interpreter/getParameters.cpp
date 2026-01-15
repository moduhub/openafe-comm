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
      // [removed] The memory of the Arduino R3 is limiting processes; the TIA and CUR settings were removed to gain more space.
      //_parseTIAParams(pCommandParams, pCommandString);
      return 1;

    case CMDID_CUR:
      // [removed] The memory of the Arduino R3 is limiting processes; the TIA and CUR settings were removed to gain more space.
      // _parseCurrentRangeParams(pCommandParams, pCommandString);
      return 1;

    case CMDID_CVW:
      _parseParams(pCommandParams, pCommandString, CMDID_CVW);
      return 1;

    case CMDID_DPV:
      _parseParams(pCommandParams, pCommandString, CMDID_DPV);
      return 1;

    case CMDID_SWV:
      _parseParams(pCommandParams, pCommandString, CMDID_SWV);
      return 1;

    case CMDID_EIS:
      _parseParams(pCommandParams, pCommandString, CMDID_EIS);
      return 1;

    default:
      return ERROR_INVALID_COMMAND;
  }
}