#include "../openAFE_Executioner.hpp"

int _executeCyclicVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams){	

	if (pOpenafeInstance){
		int tResult = pOpenafeInstance->setCVSequence(
      pCommandParams->settlingTime,
      pCommandParams->startingPotential, pCommandParams->endingPotential,
      pCommandParams->scanRate, pCommandParams->stepPotential,
      pCommandParams->numCycles
    );

		if (tResult <= 0) return ERROR_PARAM_OUT_BOUNDS;
		else {
			pinMode(2, INPUT);
			attachInterrupt(digitalPinToInterrupt(2), pOpenafeInstance->interruptHandler, LOW); 
			pOpenafeInstance->startVoltammetry();
			return STATUS_VOLTAMMETRY_UNDERGOING;
		}
	}
	else return ERROR_GENERAL;

	return ERROR_GENERAL; // JUST FOR TESTING
}