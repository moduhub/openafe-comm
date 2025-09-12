#include "../openAFE_Serial.hpp"

void send_msg_startingSpectroscoy(void){
	sendMessage("MSG,CVS");
	return;
}

void send_msg_endOfSpectroscopy(void){
	sendMessage("MSG,END");
	return;
}