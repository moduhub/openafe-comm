#include "../openAFE_Interpreter.hpp"

void _parseCurrentRangeParams(command_t *pCommandParams, String pCommandString){
	String tParamsArray[2];

	separateCommandParameters(tParamsArray, pCommandString, ',', 2);
	pCommandParams->id = CMDID_CUR;
	pCommandParams->currentRange = tParamsArray[1].toInt();

	return;
}