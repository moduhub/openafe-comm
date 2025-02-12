#include "openAFEComm.h"

#include "openAFE_Shared.h"
#include "openAFE_Error_Codes.h"
#include "openAFE_Interpreter.h"
#include "openAFE_Executioner.h"

#include <stdio.h>

#define COMM_BUFFER_SIZE 64 // The size of the communication buffer, for commands and messages.

static command_t commandParams;

AFE gOpenafeInstance;


/**
 * @brief Calculate the checksum for a given string/message.
 * 
 * @note  Note that the message passed should not include the suffix '$',
 * it must be the raw message, for example: "MSG,RDY".
 * 
 * @param pMessage IN -- Message to have its CRC calculated, without the suffix, 
 * e.g.: "MSG,RDY".
 * @return the integer CRC calculated. 
 */
uint8_t _calculateCRCForMessage(String pMessage)
{
	uint8_t tMessageLength = pMessage.length();
	
	char tMessageArray[tMessageLength + 1];

	pMessage.toCharArray(tMessageArray, tMessageLength + 1);

	uint8_t calculatedChecksum = 0;

	for (uint8_t i = 0; i < tMessageLength; i++)
	{
		calculatedChecksum ^= (char)(tMessageArray[i]);
	}

	return calculatedChecksum;
}

/**
 * @brief Sends the given message to the App, adding the suffix and CRC.
 * 
 * @note Note that the message shall not include the suffix ($) nor 
 * asterix (*), e.g.: "MSG,RDY".
 * 
 * @param pMessage IN -- Raw message to be sent, e.g.: "MSG,RDY".
 */
void _sendMessage(String pMessage)
{
	if(!Serial) return;

	char tChecksumArr[3];

	sprintf(tChecksumArr, "%02x", _calculateCRCForMessage(pMessage));

	String tChecksumString = String(tChecksumArr);
	tChecksumString.toUpperCase();

	String tMessageToSend = '$' + pMessage + '*' + tChecksumString + '\n';

	Serial.print(tMessageToSend);

	return;
}

// *** NORMAL MESSAGES
/**
 * @brief Send a message to the App aknowledging that the command was received.
 * 
 */
void _MSG_received(void)
{
	_sendMessage("MSG,RCD");
	return;
}

/**
 * @brief Send message to the App that the device is ready.
 * 
 */
void _MSG_ready(void)
{
	_sendMessage("MSG,RDY");
	return;
}

/**
 * @brief Send message to the App that the wave generation is starting.
 * 
 */
void _MSG_startingVoltammetry(void)
{
	_sendMessage("MSG,STR");
	return;
}

/**
 * @brief Send message to the App that the wave generation is finished.
 *
 */
void _MSG_endOfVoltammetry(void)
{
	_sendMessage("MSG,END");
	return;
}

/**
 * @brief Sends the proper error message accorrding to the error code passed.
 *
 * @param pErrorCode IN -- Code of the error ocurred, e.g.: -1 (ERROR_INVALID_COMMAND).
 */
void _ERR_HANDLER(int pErrorCode);

/**
 * @brief Send a single point result (SGL) to the App, with the given parameters 
 * and also check for incoming messages from the App, and send command to executioner,
 * if valid.
 * 
 * @param pVoltage_mV IN -- Voltage level in mV.
 * @param pCurrent_uA In -- Current in uA.
 */
void _sendSinglePointResult(float pVoltage_mV, float pCurrent_uA)
{
	noInterrupts();
	if (Serial.available() > 0)
	{
		// Read user input string
		String tCommandReceived = Serial.readStringUntil('\n');
		// Remove trailing newline character
		tCommandReceived.trim();

		if(openAFEInterpreter_isCommandCRCValid(tCommandReceived))
		{
			int tInterpretResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams);

			if (tInterpretResult < 0) 
			{
				_ERR_HANDLER(tInterpretResult);
			} 
			else 
			{
				int tExecutionResult = openAFEExecutioner_executeCommand(&gOpenafeInstance, &commandParams);

				if (tExecutionResult < 0) _ERR_HANDLER(tExecutionResult);
			}
		}
	}
	interrupts();

	String tSinglePointResult = "SGL," + String(pVoltage_mV) + "," + String(pCurrent_uA); 
	_sendMessage(tSinglePointResult);
}


int _handlePoint(AFE *pOpenafeInstance)
{
	do
	{

		if (pOpenafeInstance->dataAvailable() > 0)
		{
			Serial.println(SEQ0);
			float voltage_mV;
			float current_uA;
			pOpenafeInstance->getPoint(&voltage_mV, &current_uA);
			interrupts(); // Enable back interrupts after reading data from FIFO
			SEQ0++;
			_sendSinglePointResult(voltage_mV, current_uA);
		}
		delay(1);
		Serial.flush();
	} while (!pOpenafeInstance->done());

	detachInterrupt(digitalPinToInterrupt(2));

	return EXE_CVW_DONE;
}


void openAFEComm_waitForCommands(AFE *pOpenafeInstance)
{
	if(!Serial) return;

	gOpenafeInstance = *pOpenafeInstance;
	
	_MSG_ready();

	while (1)
	{
		String tCommandReceived;

		// Wait for user input
		while (Serial.available() == 0)
		{
			// do nothing
		}

		// Read user input string
		tCommandReceived = Serial.readStringUntil('\n');

		// Remove trailing newline character
		tCommandReceived.trim();

		// Check if input value is within range
		if (openAFEInterpreter_isCommandCRCValid(tCommandReceived))
		{
			openAFEExecutioner_setPointResultMessageCallback(_sendSinglePointResult);

			int tInterpreterResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams);

			if(tInterpreterResult < 0) {
				_ERR_HANDLER(tInterpreterResult);
			} 
			if (commandParams.id == CMDID_CVW || commandParams.id == CMDID_DPV || commandParams.id == CMDID_SWV)
				_MSG_received();

			int tExeResult = openAFEExecutioner_executeCommand(&gOpenafeInstance, &commandParams);

			if (tExeResult == STATUS_VOLTAMMETRY_UNDERGOING)
			{
				tExeResult = _handlePoint(&gOpenafeInstance);
			} 
			
			if (tExeResult < 0) {
				_ERR_HANDLER(tExeResult);
			} else if ((tExeResult == EXE_CVW_DONE) || (tExeResult == EXE_DPV_DONE) || (tExeResult == EXE_SWV_DONE)){
				_MSG_endOfVoltammetry();
			} else {
				_MSG_received();
			}
		}
		else
		{
			_ERR_HANDLER(ERROR_INVALID_COMMAND);
		}
	}
}

void _ERR_HANDLER(int pErrorCode)
{
	switch (pErrorCode)
	{
	case ERROR_INVALID_COMMAND:
		_sendMessage("ERR,INV");
		break;
	case ERROR_PARAM_OUT_BOUNDS:
		_sendMessage("ERR,PAR");
		break;
	case ERROR_PARAM_MISSING:
		_sendMessage("ERR,PAR");
		break;
	case ERROR_GENERAL:
		_sendMessage("ERR,GEN");
		break;
	case ERROR_AFE_NOT_WORKING:
		_sendMessage("ERR,AFE");
		break;
	case ERROR_WAVE_GEN:
		_sendMessage("ERR,WAV");
	default:
		break;
	}
}