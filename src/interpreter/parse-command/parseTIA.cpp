#include "../openAFE_Interpreter.hpp"

void _parseTIAParams(command_t *pCommandParams, String pCommandString){
	String tParamsArray[2];
	
	separateCommandParameters(tParamsArray, pCommandString, ',', 2);

	pCommandParams->id = CMDID_TIA;
	pCommandParams->gainTIA = tParamsArray[1].toInt();

	return;
}
