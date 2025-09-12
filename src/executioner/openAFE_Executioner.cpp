#include "openAFE_Executioner.hpp"

void (*gPointResultMessageCallback)(float, float);

// ==== PUBLIC ==== //

void openAFEExecutioner_setPointResultMessageCallback(void (*pPointResultMessageCallback)(float, float)){
	gPointResultMessageCallback = pPointResultMessageCallback;
}

int openAFEExecutioner_executeCommand(AFE *pOpenafeInstance, command_t *pCommandParams){

	switch (pCommandParams->id){
	case CMDID_CHK:
		return _checkAFEHealth(pOpenafeInstance);
		break;
	
	case CMDID_DIE:
		return killProcess(pOpenafeInstance);
		break;

	case CMDID_TIA:
		return _setTIAGainResistor(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_CVW:
		return _executeCyclicVoltammetry(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_DPV:
		return _executeDifferentialPulseVoltammetry(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_SWV:
		return _executeSquareWaveVoltammetry(pOpenafeInstance, pCommandParams);
		break;

  case CMDID_EIS:
		return _executeImpedanceSpectroscopy(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_CUR:
		return _setCurrentRange(pOpenafeInstance, pCommandParams);
		break;

	default:
		return ERROR_INVALID_COMMAND;
		break;
	}
}

// ====        ==== //



// ==== PRIVATE ==== //

int _checkAFEHealth(AFE *pOpenafeInstance){
	if(pOpenafeInstance)
	{
		if (pOpenafeInstance->isAFEResponding()) {
			return 1;
		} else {
			return ERROR_AFE_NOT_WORKING;
		}
	} else {
		return ERROR_AFE_NOT_WORKING;
	}
	return ERROR_AFE_NOT_WORKING;
}

int killProcess(AFE *pOpenafeInstance){
	if (pOpenafeInstance) pOpenafeInstance->killVoltammetry();
  delay(100);
  asm volatile ("  jmp 0");
	return 1;
}

void _handlePointResult(float pVoltage_mV, float pCurrent_uA){
	gPointResultMessageCallback(pVoltage_mV, pCurrent_uA);
}

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
			return STATUS_VOLTAMMETRY_UNDERGOING;
		}
	}
	else return ERROR_GENERAL;
}

int _executeSquareWaveVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams){
	if (pOpenafeInstance){
		uint8_t tResult = pOpenafeInstance->setSWVSequence(
      pCommandParams->settlingTime,
      pCommandParams->startingPotential, pCommandParams->endingPotential,
      pCommandParams->scanRate, pCommandParams->stepPotential, pCommandParams->pulsePotential,
      pCommandParams->dutyCycle 
    );

		if (tResult <= 0) return ERROR_PARAM_OUT_BOUNDS;
		else{
			pinMode(2, INPUT);
			attachInterrupt(digitalPinToInterrupt(2), pOpenafeInstance->interruptHandler, LOW); 
			pOpenafeInstance->startVoltammetry();
			return STATUS_VOLTAMMETRY_UNDERGOING;
		}
	}
	else return ERROR_GENERAL;
}

int _executeImpedanceSpectroscopy(AFE *pOpenafeInstance, command_t *pCommandParams){
  if (pOpenafeInstance)
	{
		int tResult; /*= pOpenafeInstance->setCVSequence(
			pCommandParams->settlingTime,
			pCommandParams->startingOmega,
			pCommandParams->endingOmega,
			pCommandParams->stepForADecade,
      pCommandParams->scanRate);*/

		if (tResult <= 0) {
			return ERROR_PARAM_OUT_BOUNDS;
		} else {
			pinMode(2, INPUT);
			attachInterrupt(digitalPinToInterrupt(2), pOpenafeInstance->interruptHandler, LOW); // Config the Arduino Interrupt

			//pOpenafeInstance->startSpectroscopy();

			return STATUS_SPECTROSCOPY_UNDERGOING;
		}
	}
	else
	{
		return ERROR_GENERAL;
	}

	return ERROR_GENERAL; // JUST FOR TESTING
}

int _setTIAGainResistor(AFE *pOpenafeInstance, command_t *pCommandParams){
	if (pOpenafeInstance)
	{
		pOpenafeInstance->setTIAGain(pCommandParams->gainTIA);
		return CMDID_TIA_SETTED;
	} else {
		return ERROR_GENERAL;
	}
}

int _setCurrentRange(AFE *pOpenafeInstance, command_t *pCommandParams){
	if (pOpenafeInstance)
	{
		pOpenafeInstance->setCurrentRange(pCommandParams->currentRange);
		return CMDID_CUR_SETTED;
	} else {
		return ERROR_GENERAL;
	}
}

// ====         ==== //