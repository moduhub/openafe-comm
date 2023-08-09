#include "openAFE_Executioner.h"

#include "openAFE_Shared.h"
#include "openAFE_Error_Codes.h"

#include "Arduino.h"
// #include <stdint.h>

/**
 * @brief Check the value in the AFE IC ADIID register. If the AFE IC does
 * not give the expected 0x4144 then something is wrong. 
 * 
 * @return >0 if successful, error code on error. 
 */
int _checkAFEHealth(void);

/**
 * @brief Executes the Cyclic Voltammetry with the received parameters.
 * The function will return error code in case of any error. 
 * 
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _executeCyclicVoltammetry(command_t *pCommandParams);

int openAFEExecutioner_executeCommand(command_t *pCommandParams)
{
	// DEBUG:
	// Serial.print("EXE command ID: ");
	// Serial.println(pCommandParams.id);

	switch (pCommandParams->id)
	{
	case CMDID_CHK:
		return _checkAFEHealth();
		break;
	
	case CMDID_CVW:
		return _executeCyclicVoltammetry(pCommandParams);
		break;

	default:
		return ERROR_INVALID_COMMAND;
		break;
	}
}

int _checkAFEHealth(void)
{
	if(Serial){
		Serial.println("Ran AFE check!");
	}
	// call the openAFE AFE check
	return 1; // JUST FOR TESTING
}

int _executeCyclicVoltammetry(command_t *pCommandParams)
{
	Serial.print("Ending Potential: ");
	Serial.println(pCommandParams->endingPotential);

	Serial.print("Starting Potential: ");
	Serial.println(pCommandParams->startingPotential);

	Serial.print("Scan Rate: ");
	Serial.println(pCommandParams->scanRate);

	Serial.print("Step Size: ");
	Serial.println(pCommandParams->stepSize);

	Serial.print("Number of Cycles: ");
	Serial.println(pCommandParams->numCycles);

	Serial.println("EXE Executing CV");
	return 1; // JUST FOR TESTING
}