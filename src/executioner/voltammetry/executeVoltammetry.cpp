#include "../openAFE_Executioner.hpp"

int _executeVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams){
	if (pOpenafeInstance){
    uint8_t tResult;

    switch (pCommandParams->id){
      case CMDID_CVW:
        tResult = pOpenafeInstance->setCVSequence(
          pCommandParams->settlingTime,
          pCommandParams->startingPotential, pCommandParams->endingPotential,
          pCommandParams->scanRate, pCommandParams->stepPotential,
          pCommandParams->numCycles
        );
        break;

      case CMDID_DPV:
        tResult = pOpenafeInstance->setDPVSequence(
          pCommandParams->settlingTime,
          pCommandParams->startingPotential, pCommandParams->endingPotential,
          pCommandParams->scanRate, pCommandParams->stepPotential, pCommandParams->pulsePotential,
          pCommandParams->dutyCycle 
        );
        break;

      case CMDID_SWV:
        tResult = pOpenafeInstance->setSWVSequence(
          pCommandParams->settlingTime,
          pCommandParams->startingPotential, pCommandParams->endingPotential,
          pCommandParams->scanRate, pCommandParams->stepPotential, pCommandParams->pulsePotential,
          pCommandParams->dutyCycle 
        );
        break;
    }		

		if (tResult <= 0) return ERROR_PARAM_OUT_BOUNDS;
		else{
			pinMode(2, INPUT);
			attachInterrupt(digitalPinToInterrupt(2), pOpenafeInstance->interruptHandler, LOW); 
			pOpenafeInstance->startVoltammetry();
      interrupts();
			return STATUS_VOLTAMMETRY_UNDERGOING;
		}
	}
	else return ERROR_GENERAL;
}