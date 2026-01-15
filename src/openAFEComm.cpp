#include "openAFEComm.hpp"

static command_t commandParams;

AFE openafe;

void openAFEComm_waitForCommands(void){
	if(!Serial) return;
	
  sendStatus(ST_MSG_READY);

  //  [WP] Send EIS example
  // $EIS,1000,20,60000,10*
  /*
  String pMessage = "EIS,1000,20,60000,10";
  char tChecksumArr[3];
  sprintf(tChecksumArr, "%02x", calculateCRC(pMessage));
	String tChecksumString = String(tChecksumArr);
	tChecksumString.toUpperCase();
	String tMessageToSend = '$' + pMessage + '*' + tChecksumString + '\n';
  Serial.print(tMessageToSend);
  */
  
	while (1){
		String tCommandReceived;

		while (Serial.available() == 0); // Wait for user input
		tCommandReceived = Serial.readStringUntil('\n'); // Read user input string
		tCommandReceived.trim(); // Remove trailing newline character

		if (checkCRC(tCommandReceived)){
      
			openAFEExecutioner_setPointResultMessageCallback([]
        (
          int cmdId, 
          float voltage, float current_1, float current_2, 
          float frequency, float impedance_real, float impedance_imag
        ) {
        if (cmdId == CMDID_CVW) {
          sendPoint(PT_CV, voltage, current_1, 0.0f); //sendPointCV(voltage, current_1);
        } else if (cmdId == CMDID_DPV) {
          sendPoint(PT_DPV, voltage, current_1, current_2); //sendPointDPV(voltage, current_1, current_2);
        } else if (cmdId == CMDID_SWV) {
          sendPoint(PT_SWV, voltage, current_1, current_2); //sendPointSW(voltage, current_1, current_2);
        } else if (cmdId == CMDID_EIS) {
          sendPoint(PT_EIS, frequency, impedance_real, impedance_imag); //sendPointEIS(frequency, impedance_real, impedance_imag);
        }
      });

			int tInterpreterResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams);

			if(tInterpreterResult < 0) 
				sendStatus((status_id_t)tInterpreterResult);
			if (commandParams.id == CMDID_CVW || commandParams.id == CMDID_DPV || commandParams.id == CMDID_SWV || commandParams.id == CMDID_EIS)
				sendStatus(ST_MSG_RECEIVED);

			int tExeResult = openAFEExecutioner_executeCommand(&openafe, &commandParams);
      
			if (tExeResult == STATUS_VOLTAMMETRY_UNDERGOING) {
        sendStatus(ST_MSG_VOLTAMMETRY_START);
        tExeResult = handlePoint(&openafe, &commandParams);
      }
      else if(tExeResult == STATUS_SPECTROSCOPY_UNDERGOING){
        sendStatus(ST_MSG_EIS_START);
        tExeResult = handlePointEIS(&openafe, &commandParams);
      }
      
      Serial.flush();
      delay(30);
			if (tExeResult < 0) sendStatus((status_id_t)tExeResult);
			else if (tExeResult == EXE_CVW_DONE)    sendStatus(ST_MSG_END);
      else if (tExeResult == EXE_EIS_DONE)    sendStatus(ST_MSG_END);
      else if(tExeResult == CMDID_CUR_SETTED) sendStatus(ST_MSG_TIA_UPDT);
      else if(tExeResult == CMDID_TIA_SETTED) sendStatus(ST_MSG_CUR_UPDT);
      else sendStatus(ST_MSG_RECEIVED);
		}
		else sendStatus((status_id_t)ERROR_INVALID_COMMAND);
	}
}