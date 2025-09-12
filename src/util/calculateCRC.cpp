#include "CRC.hpp"

uint8_t calculateCRC(const String& pMessage) {
  uint8_t crc = 0;

  for (uint8_t i = 0; i < pMessage.length(); i++)
    crc ^= static_cast<uint8_t>(pMessage[i]);
  
  return crc;
}