#include "openAFEComm.h"

#include "openAFE_Shared.h"
#include "openAFE_Error_Codes.h"
#include "openAFE_Interpreter.h"
#include "openAFE_Executioner.h"

#include <stdio.h>

#define COMM_BUFFER_SIZE 64 // The size of the communication buffer, for commands and messages.

/**
 * @brief Get the interger checksum from a string/message.
 *
 * @param pString IN -- String with the CRC-8 to be stracted, e.g.: "$CV,500,-500,250,10,2*10"
 * @return CRC-8 integer (e.g. 16).
 */
uint8_t _getCRCIntegerFromString(String pString)
{
	uint8_t tStringLength = pString.length();

	char tStringCheckSum[] = {pString[tStringLength - 2], pString[tStringLength - 1]};
	uint8_t tIntegerChecksum;

	tIntegerChecksum = tStringCheckSum[1] > 0x3a ? (uint8_t)(tStringCheckSum[1] - 0x37) : (uint8_t)(tStringCheckSum[1] - 0x30);
	tIntegerChecksum += tStringCheckSum[0] << 4;

	// Serial.print("Integer cehcksum: 0x");
	// Serial.println(tIntegerChecksum, HEX);

	return tIntegerChecksum;
}

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
 * @brief Check if the passed command has a valid CRC of not.
 *
 * @param pCommand IN -- Command string to be checked, e.g.: "$CV,500,-500,250,10,2*10".
 * @return True if command is valid, false if not valid.
 */
bool _isCommandCRCValid(String pCommand)
{
	uint8_t tCommandLength = pCommand.length();

	char tCommandArray[tCommandLength + 1];

	pCommand.toCharArray(tCommandArray, tCommandLength + 1);

	uint8_t tCalculatedChecksum = 0;

	// the iterator starts at 1 to eliminate the suffix, '$'
	// the '-3' is to remove the asterix and the CRC, "*7C"
	for (uint8_t i = 1; i < tCommandLength - 3; i++)
	{
		// Serial.print(tCommandArray[i]);
		tCalculatedChecksum ^= tCommandArray[i];
	}

	uint8_t tChecksumFromCommand = _getCRCIntegerFromString(pCommand);

	bool isCommandValid = (tCalculatedChecksum - tChecksumFromCommand) == 0 ? true : false;

	return isCommandValid;
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

	String tMessageToSend = '$' + pMessage + '*' + tChecksumString;

	Serial.println(tMessageToSend);

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

// *** ERROR MESSAGES
/**
 * @brief Send GENERAL ERROR to the App.
 * 
 */
void _ERR_GENERAL(void)
{
	_sendMessage("ERR,GEN");
	return;
}

/**
 * @brief Send INVALID ERROR to the App.
 * 
 */
void _ERR_INVALID_CHECKSUM(void)
{
	_sendMessage("ERR,INV");
	return;
}

/**
 * @brief Send PARAMETER ERROR to the App.
 * 
 */
void _ERR_PARAMETER_OUT_OF_BOUNDS(void)
{
	_sendMessage("ERR,PAR");
	return;
}

/**
 * @brief Send WAVE ERROR to the App.
 * 
 */
void _ERR_WAVE_GENERATION(void)
{
	_sendMessage("ERR,WAV");
	return;
}

/**
 * @brief Send AFE ERROR to the App.
 * 
 */
void _ERR_AFE_NOT_WORKING(void)
{
	_sendMessage("ERR,AFE");
	return;
}

static command_t commandParams;

void openAFEComm_waitForCommands(void)
{
	if(!Serial) return;

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
		if (_isCommandCRCValid(tCommandReceived))
		{
			// Serial.println("Command is valid!");

			if(openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams) < 0)
				_ERR_GENERAL();

			_MSG_received();

			int tExeResult = openAFEExecutioner_executeCommand(&commandParams);

			if (tExeResult < 0) 
				_ERR_GENERAL(); 
		}
		else
		{
			_ERR_INVALID_CHECKSUM();
		}
	}
}