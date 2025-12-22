#include "../openAFE_Executioner.hpp"

int _executeImpedanceSpectroscopy(AFE *pOpenafeInstance, command_t *pCommandParams){
  if (pOpenafeInstance){
		int tResult = pOpenafeInstance->setEISConfig(
      pCommandParams->settlingTime,
      pCommandParams->startingOmega,
      pCommandParams->endingOmega,
      pCommandParams->stepForADecade
    );

		if (tResult <= 0) {
			return ERROR_PARAM_OUT_BOUNDS;
		} else {
			pinMode(2, INPUT_PULLUP);
			attachInterrupt(digitalPinToInterrupt(2), pOpenafeInstance->interruptHandler_EIS, FALLING); // Config the Arduino Interrupt
      delay(10);
      pOpenafeInstance->startEIS();
      interrupts();
      return STATUS_SPECTROSCOPY_UNDERGOING;
		}
	}
	else
	{
		return ERROR_GENERAL;
	}

	return ERROR_GENERAL;
}