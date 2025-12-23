#include "../openAFE_Executioner.hpp"

int _executeDifferentialPulseVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams){

	if (pOpenafeInstance){
		uint8_t tResult = pOpenafeInstance->setDPVSequence(
      pCommandParams->settlingTime,
      pCommandParams->startingPotential, pCommandParams->endingPotential,
      pCommandParams->scanRate, pCommandParams->stepPotential, pCommandParams->pulsePotential,
      pCommandParams->dutyCycle 
    );

		if (tResult <= 0) return ERROR_PARAM_OUT_BOUNDS;
		else {
			pinMode(2, INPUT);
			attachInterrupt(digitalPinToInterrupt(2), pOpenafeInstance->interruptHandler, LOW); 
			pOpenafeInstance->startVoltammetry();
      interrupts();
			return STATUS_VOLTAMMETRY_UNDERGOING;
		}
	}
	else return ERROR_GENERAL;
}