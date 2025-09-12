#include "../openAFE_Serial.hpp"

void sendMessage(String pMessage){
	if(!Serial) return;

	char tChecksumArr[3];

	sprintf(tChecksumArr, "%02x", calculateCRC(pMessage));

	String tChecksumString = String(tChecksumArr);
	tChecksumString.toUpperCase();

	String tMessageToSend = '$' + pMessage + '*' + tChecksumString + '\n';

	Serial.print(tMessageToSend);
  Serial.flush();

	return;
}