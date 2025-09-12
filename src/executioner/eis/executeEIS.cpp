#include "../openAFE_Executioner.hpp"

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