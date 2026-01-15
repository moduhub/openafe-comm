#include "../openAFE_Serial.hpp"

void sendMessage(const char *msg){
  if (!Serial) return;

  uint8_t crc = calculateCRC(msg);

  Serial.write('$');
  Serial.print(msg);
  Serial.write('*');

  const char hex[] = "0123456789ABCDEF";
  Serial.write(hex[(crc >> 4) & 0x0F]);
  Serial.write(hex[crc & 0x0F]);

  Serial.write('\n');
  Serial.flush();

  return;
}
