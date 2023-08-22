#include "openAFE_Executioner.h"

#include "openAFE.h"
#include "openAFE_Shared.h"
#include "openAFE_Error_Codes.h"

#include "Arduino.h"
// #include <stdint.h>

void (*gPointResultMessageCallback)(float, float);

/**
 * @brief Check the value in the AFE IC ADIID register. If the AFE IC does
 * not give the expected 0x4144 then something is wrong. 
 * 
 * @return >0 if successful, error code on error. 
 */
int _checkAFEHealth(AFE *pOpenafeInstance);

/**
 * @brief Stop the currently undergoing voltammetry.
 * 
 * @param pOpenafeInstance IN -- OpenAFE class instance.
 * @return 
 */
int _stopVoltammetry(AFE *pOpenafeInstance);

/**
 * @brief Handle the received point result from voltammetry proccess.
 * 
 * @param pVoltage_mV IN -- Voltage level in millivolts.
 * @param pCurrent_uA IN -- Current in microAmps.
 */
void _handlePointResult(float pVoltage_mV, float pCurrent_uA);

/**
 * @brief Executes the Cyclic Voltammetry with the received parameters.
 * The function will return error code in case of any error. 
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _executeCyclicVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief Set the TIA Gain Resistor.
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _setTIAGainResistor(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief Set the current range of the AFE.
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _setCurrentRange(AFE *pOpenafeInstance, command_t *pCommandParams);


void openAFEExecutioner_setPointResultMessageCallback(void (*pPointResultMessageCallback)(float, float))
{
	gPointResultMessageCallback = pPointResultMessageCallback;
}

int openAFEExecutioner_executeCommand(AFE *pOpenafeInstance, command_t *pCommandParams)
{
	// DEBUG:
	// Serial.print("EXE command ID: ");
	// Serial.println(pCommandParams.id);

	switch (pCommandParams->id)
	{
	case CMDID_CHK:
		return _checkAFEHealth(pOpenafeInstance);
		break;
	
	case CMDID_DIE:
		return _stopVoltammetry(pOpenafeInstance);
		break;

	case CMDID_TIA:
		return _setTIAGainResistor(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_CVW:
		return _executeCyclicVoltammetry(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_CUR:
		return _setCurrentRange(pOpenafeInstance, pCommandParams);
		break;

	default:
		return ERROR_INVALID_COMMAND;
		break;
	}
}

int _checkAFEHealth(AFE *pOpenafeInstance)
{
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

int _stopVoltammetry(AFE *pOpenafeInstance)
{
	if (pOpenafeInstance)
	{
		pOpenafeInstance->killVoltammetry();
	}
	return 1;
}

void _handlePointResult(float pVoltage_mV, float pCurrent_uA)
{
	gPointResultMessageCallback(pVoltage_mV, pCurrent_uA);
}

int _executeCyclicVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams)
{	
	if (pOpenafeInstance)
	{
		pOpenafeInstance->setPointResultCallback(_handlePointResult);
	}

	// Serial.print("Ending Potential: ");
	// Serial.println(pCommandParams->endingPotential);

	// Serial.print("Starting Potential: ");
	// Serial.println(pCommandParams->startingPotential);

	// Serial.print("Scan Rate: ");
	// Serial.println(pCommandParams->scanRate);

	// Serial.print("Step Size: ");
	// Serial.println(pCommandParams->stepSize);

	// Serial.print("Number of Cycles: ");
	// Serial.println(pCommandParams->numCycles);

	if (pOpenafeInstance)
	{
		uint8_t tCVResult = pOpenafeInstance->waveformCV(pCommandParams->endingPotential / 1000,
											   pCommandParams->startingPotential / 1000,
											   pCommandParams->scanRate,
											   pCommandParams->stepSize,
											   pCommandParams->numCycles);
		
		if (!tCVResult) {
			return ERROR_PARAM_OUT_BOUNDS;
		} else {
			return 1;
		}
	}
	else
	{
		return ERROR_GENERAL;
	}

	return ERROR_GENERAL; // JUST FOR TESTING
}

int _setTIAGainResistor(AFE *pOpenafeInstance, command_t *pCommandParams)
{
	if (pOpenafeInstance)
	{
		pOpenafeInstance->setTIAGain(pCommandParams->gainTIA);
		return 1;
	} else {
		return ERROR_GENERAL;
	}
}

int _setCurrentRange(AFE *pOpenafeInstance, command_t *pCommandParams)
{
	if (pOpenafeInstance)
	{
		pOpenafeInstance->setCurrentRange(pCommandParams->currentRange);
		return 1;
	} else {
		return ERROR_GENERAL;
	}
}
