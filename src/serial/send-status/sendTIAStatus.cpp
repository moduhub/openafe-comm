#include "../openAFE_Serial.hpp"

void send_msg_TIAUpdate(void){
	sendMessage("MSG,TIA,UPDT");
	return;
}