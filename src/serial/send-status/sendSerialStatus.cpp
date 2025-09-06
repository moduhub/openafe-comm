#include "../openAFE_Serial.hpp"

void send_msg_received(void){
	sendMessage("MSG,RCD");
	return;
}

void send_ready(void){
	sendMessage("MSG,RDY");
	return;
}