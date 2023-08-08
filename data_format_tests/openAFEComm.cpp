#include "openAFEComm.h"

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
		// printf("%c", (char)tMessageArray[i]);
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

void openAFEComm_waitForCommands(void)
{
	// message send, ready
	Serial.print("$MSG,RDY*");
	Serial.println((int)_calculateCRCForMessage("MSG,RDY"), HEX);

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
			Serial.println("Command is valid!");
		}
		else
		{
			Serial.println("Error: Invalid command received. Please enter a valid command!");
		}
	}
}