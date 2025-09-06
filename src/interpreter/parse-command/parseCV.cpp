#include "../openAFE_Interpreter.hpp"

void _parseCVParams(command_t *pCommandParams, String pCommandString){

	String tParamArray[6];

	separateCommandParameters(tParamArray, pCommandString, ',', 6);

	pCommandParams->id = CMDID_CVW;
	pCommandParams->settlingTime = tParamArray[0].toInt();
	pCommandParams->startingPotential = tParamArray[1].toFloat();
	pCommandParams->endingPotential = tParamArray[2].toFloat();
	pCommandParams->scanRate = tParamArray[3].toFloat();
	pCommandParams->stepPotential = tParamArray[4].toFloat();
	pCommandParams->numCycles = tParamArray[5].toInt();

	return;
}