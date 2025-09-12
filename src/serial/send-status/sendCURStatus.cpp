#include "../openAFE_Serial.hpp"

void send_msg_CURUpdate(void){
	sendMessage("MSG,CUR,UPDT");
	return;
}