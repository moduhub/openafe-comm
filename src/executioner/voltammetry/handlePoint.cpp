#include "../openAFE_Executioner.hpp"

void (*gPointResultMessageCallback)(int cmdId, float v1, float c1, float c2);

void _handlePointResult(int cmdId, float vpVoltage_mV1, float pCurrent_uA, float pCurrent_uA_2){
  if (gPointResultMessageCallback) {
    gPointResultMessageCallback(cmdId, vpVoltage_mV1, pCurrent_uA, pCurrent_uA_2);
  }
}

void openAFEExecutioner_setPointResultMessageCallback(void (*pPointResultMessageCallback)(int, float, float, float)) {
  gPointResultMessageCallback = pPointResultMessageCallback;
}

int handlePoint(AFE *pOpenafeInstance, command_t *commandParams){
  char cmdBuf[128];
  size_t cmdIdx = 0;

  do {
    if (pOpenafeInstance->dataAvailable() > 0) {
      noInterrupts();
      float voltage_mV;
      float currents_uA[2];
      pOpenafeInstance->getPoint(&voltage_mV, currents_uA);
      interrupts();

      _handlePointResult(commandParams->id, voltage_mV, currents_uA[0], currents_uA[1]);
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
            int tInterpretResult = openAFEInterpreter_getParametersFromCommand(tCommandReceived, commandParams);
            if (tInterpretResult < 0) {
              sendError(tInterpretResult);
            } else {
              int tExecutionResult = openAFEExecutioner_executeCommand(pOpenafeInstance, commandParams);
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

  Serial.flush();
  delay(10);// GArantir que todos os dados serão enviados pela serial

  detachInterrupt(digitalPinToInterrupt(2));
  return EXE_CVW_DONE;
}