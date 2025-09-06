#include "../openAFE_Serial.hpp"

void send_endOfSpectroscopy(void){
	sendMessage("MSG,END");
	return;
}