#include "../openAFE_Interpreter.hpp"

void _parseDPVParams(command_t *pCommandParams, String pCommandString){
  String tParamArray[7];

  separateCommandParameters(tParamArray, pCommandString, ',', 7);

  pCommandParams->id = CMDID_DPV;
  pCommandParams->settlingTime = tParamArray[0].toInt();
  pCommandParams->startingPotential = tParamArray[1].toFloat();
  pCommandParams->endingPotential = tParamArray[2].toFloat();
  pCommandParams->scanRate = tParamArray[3].toFloat();
  pCommandParams->stepPotential = tParamArray[4].toFloat();
  pCommandParams->pulsePotential = tParamArray[5].toFloat();
  pCommandParams->dutyCycle = tParamArray[6].toFloat();

  return;
}