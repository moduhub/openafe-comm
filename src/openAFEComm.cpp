#include "openAFEComm.hpp"

static command_t commandParams;

AFE openafe;

void sendPoint(float pVoltage_mV, float pCurrent_uA){
	noInterrupts();
	if (Serial.available() > 0){
		// Read user input string
		String tCommandReceived = Serial.readStringUntil('\n');
		// Remove trailing newline character
		tCommandReceived.trim();

		if(checkCRC(tCommandReceived)){
			int tInterpretResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams);

			if (tInterpretResult < 0) sendError(tInterpretResult);
			else {
				int tExecutionResult = openAFEExecutioner_executeCommand(&openafe, &commandParams);
				if (tExecutionResult < 0) sendError(tExecutionResult);
			}
		}
	}
	interrupts();

	String tSinglePointResult = "SGL," + String(pVoltage_mV) + "," + String(pCurrent_uA); 
	sendMessage(tSinglePointResult);
}

int handlePoint(AFE *pOpenafeInstance){
	do{
		if (pOpenafeInstance->dataAvailable() > 0){
			noInterrupts(); // Disable interrupts while reading data FIFO

			float voltages[2];
      float currents[2];
      pOpenafeInstance->getPoint(voltages, currents);

			interrupts(); // Enable back interrupts after reading data from FIFO

			sendPoint(voltages[0], currents[0]);
      if(commandParams.id == CMDID_DPV || commandParams.id == CMDID_SWV)
        sendPoint(voltages[1], currents[1]);
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

		while (Serial.available() == 0);                 // Wait for user input
		tCommandReceived = Serial.readStringUntil('\n'); // Read user input string
		tCommandReceived.trim();                         // Remove trailing newline character

		if (checkCRC(tCommandReceived)){
      
			openAFEExecutioner_setPointResultMessageCallback(sendPoint);

			int tInterpreterResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, &commandParams);

			if(tInterpreterResult < 0) 
				sendError(tInterpreterResult);
			if (commandParams.id == CMDID_CVW || commandParams.id == CMDID_DPV || commandParams.id == CMDID_SWV || commandParams.id == CMDID_EIS)
				send_msg_received();

			int tExeResult = openAFEExecutioner_executeCommand(&openafe, &commandParams);

			if (tExeResult == STATUS_VOLTAMMETRY_UNDERGOING) 
        tExeResult = handlePoint(&openafe);
      else if (tExeResult == STATUS_SPECTROSCOPY_UNDERGOING); 
      //  tExeResult = handlePointImpedance(&openafe);
      
			if (tExeResult < 0) sendError(tExeResult);
			else if (tExeResult == EXE_CVW_DONE) send_msg_endOfVoltammetry();
      else if (tExeResult == EXE_EIS_DONE) send_endOfSpectroscopy();
      else send_msg_received();
			
		}
		else sendError(ERROR_INVALID_COMMAND);
	}
}