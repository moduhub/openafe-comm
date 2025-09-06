#include "CRC.hpp"

bool checkCRC(const String& pCommand) {
  uint8_t crc = 0;
  
  for (uint8_t i = 1; i < pCommand.length() - 3; i++)
    crc ^= static_cast<uint8_t>(pCommand[i]);

  uint8_t hi = pCommand[pCommand.length() - 2];
  uint8_t lo = pCommand[pCommand.length() - 1];

  hi = hi > '9' ? hi - '7' : hi - '0'; 
  lo = lo > '9' ? lo - '7' : lo - '0';

  return crc == ((hi << 4) | lo);
}