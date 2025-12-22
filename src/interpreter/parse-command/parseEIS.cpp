#include "../openAFE_Interpreter.hpp"

void _parseEISParams(command_t *pCommandParams, String pCommandString){
	String tParamArray[5];

	separateCommandParameters(tParamArray, pCommandString, ',', 4);

	pCommandParams->id = CMDID_EIS;
	pCommandParams->settlingTime = tParamArray[0].toInt();
	pCommandParams->startingOmega = tParamArray[1].toInt();
	pCommandParams->endingOmega = tParamArray[2].toInt();
	pCommandParams->stepForADecade = tParamArray[3].toInt();
  
	return;
}