#include "Arduino.h"

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
uint8_t calculateCRC(const String& pMessage);

/**
 * @brief Check if the passed command has a valid CRC of not.
 *
 * @param pCommand IN -- Command string to be checked, e.g.: "$CV,500,-500,250,10,2*10".
 * @return True if command is valid, false if not valid.
 */
bool checkCRC(const String& pCommand);