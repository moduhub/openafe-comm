#include "openAFEComm.hpp"

static command_t commandParams;

AFE openafe;

void sendPoint(float pVoltage_mV, float pCurrent_uA){
  String tSinglePointResult = "SGL," + String(pVoltage_mV) + "," + String(pCurrent_uA);
  sendMessage(tSinglePointResult);
}

int handlePoint(AFE *pOpenafeInstance){
  char cmdBuf[128];
  size_t cmdIdx = 0;

  do {
    if (pOpenafeInstance->dataAvailable() > 0) {
      noInterrupts();
      float voltages[2];
      float currents[2];
      pOpenafeInstance->getPoint(voltages, currents);
      interrupts();

      sendPoint(voltages[0], currents[0]);
      if (commandParams.id == CMDID_DPV || commandParams.id == CMDID_SWV)
        sendPoint(voltages[1], currents[1]);
    }

    while (Serial.available() > 0) {
      int c = Serial.read();
      if (c < 0) break;
      if (c == '\r') continue;       
      if (c == '\n') {
        cmdBuf[cmdIdx] = '\0';
        if (cmdIdx > 0) {
          String tCommandReceived = String(cmdBuf);
          tCommandReceived.trim();
          if (checkCRC(tCommandReceived)) {
            int tInterpretResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams);
            if (tInterpretResult < 0) {
              sendError(tInterpretResult);
            } else {
              int tExecutionResult = openAFEExecutioner_executeCommand(&openafe, &commandParams);
              if (tExecutionResult < 0) sendError(tExecutionResult);
            }
          } else {
            sendError(ERROR_INVALID_COMMAND);
          }
        }
        cmdIdx = 0;
      } else {
          if (cmdIdx < sizeof(cmdBuf) - 1) {
              cmdBuf[cmdIdx++] = (char)c;
          } else {
              cmdIdx = 0;
          }
      }
    }

    delay(1);
  } while (!pOpenafeInstance->done());

  detachInterrupt(digitalPinToInterrupt(2));
  return EXE_CVW_DONE;
}

void openAFEComm_waitForCommands(void){
	if(!Serial) return;
	
	send_ready();
  
	while (1){
		String tCommandReceived;

		while (Serial.available() == 0); // Wait for user input
		tCommandReceived = Serial.readStringUntil('\n'); // Read user input string
		tCommandReceived.trim(); // Remove trailing newline character

		if (checkCRC(tCommandReceived)){
      
			openAFEExecutioner_setPointResultMessageCallback(sendPoint);

			int tInterpreterResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams);

			if(tInterpreterResult < 0) 
				sendError(tInterpreterResult);
			if (commandParams.id == CMDID_CVW || commandParams.id == CMDID_DPV || commandParams.id == CMDID_SWV || commandParams.id == CMDID_EIS)
				send_msg_received();

			int tExeResult = openAFEExecutioner_executeCommand(&openafe, &commandParams);

			if (tExeResult == STATUS_VOLTAMMETRY_UNDERGOING) {
        send_msg_startingVoltammetry();
        tExeResult = handlePoint(&openafe);
      }
      //else if (tExeResult == STATUS_SPECTROSCOPY_UNDERGOING); 
      // send_msg_startingSpectroscoy();
      //  tExeResult = handlePointImpedance(&openafe);
      
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