#include "openAFEComm.hpp"

static command_t commandParams;

AFE openafe;

void openAFEComm_waitForCommands(void){
	if(!Serial) return;
	
	send_ready();
  
	while (1){
		String tCommandReceived;

		while (Serial.available() == 0); // Wait for user input
		tCommandReceived = Serial.readStringUntil('\n'); // Read user input string
		tCommandReceived.trim(); // Remove trailing newline character

		if (checkCRC(tCommandReceived)){
      
			openAFEExecutioner_setPointResultMessageCallback([](int cmdId, float v1, float c1, float c2) {
        if (cmdId == CMDID_CVW) {
          sendPointCV(v1, c1);
        } else if (cmdId == CMDID_DPV) {
          sendPointDPV(v1, c1, c2);
        } else if (cmdId == CMDID_SWV) {
          sendPointSW(v1, c1, c2);
        }
      });

			int tInterpreterResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams);

			if(tInterpreterResult < 0) 
				sendError(tInterpreterResult);
			if (commandParams.id == CMDID_CVW || commandParams.id == CMDID_DPV || commandParams.id == CMDID_SWV || commandParams.id == CMDID_EIS)
				send_msg_received();

			int tExeResult = openAFEExecutioner_executeCommand(&openafe, &commandParams);

			if (tExeResult == STATUS_VOLTAMMETRY_UNDERGOING) {
        send_msg_startingVoltammetry();
        tExeResult = handlePoint(&openafe, &commandParams);
      }
      //else if (tExeResult == STATUS_SPECTROSCOPY_UNDERGOING); 
      // send_msg_startingSpectroscoy();
      //  tExeResult = handlePointImpedance(&openafe);
      
      Serial.flush();
      delay(30);
			if (tExeResult < 0) sendError(tExeResult);
			else if (tExeResult == EXE_CVW_DONE) send_msg_endOfVoltammetry();
      //else if (tExeResult == EXE_EIS_DONE) send_msg_endOfSpectroscopy();
      else if(tExeResult == CMDID_CUR_SETTED) send_msg_CURUpdate();
      else if(tExeResult == CMDID_TIA_SETTED) send_msg_TIAUpdate();
      else send_msg_received();
		}
		else sendError(ERROR_INVALID_COMMAND);
	}
}