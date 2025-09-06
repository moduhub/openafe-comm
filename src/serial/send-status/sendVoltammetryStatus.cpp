#include "../openAFE_Serial.hpp"

void send_msg_startingVoltammetry(void){
	sendMessage("MSG,STR");
	return;
}

void send_msg_endOfVoltammetry(void){
	sendMessage("MSG,END");
	return;
}