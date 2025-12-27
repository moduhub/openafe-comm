#include "openAFE_Executioner.hpp"

void (*gPointResultMessageCallback)(
  int cmdId, 
  float voltage, float current_1, float current_2,
  float frequency, float impedance_real, float impedance_imag
);

void _handlePointResult(
  int cmdId, 
  float voltage, float current_1, float current_2,
  float frequency, float impedance_real, float impedance_imag
){
  if (gPointResultMessageCallback) {
    gPointResultMessageCallback(
      cmdId, 
      voltage, current_1, current_2,
      frequency, impedance_real, impedance_imag
    );
  }
}

void openAFEExecutioner_setPointResultMessageCallback(void (*pPointResultMessageCallback)(
  int, 
  float, float, float, 
  float, float, float
)) {
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

      _handlePointResult(
        commandParams->id, 
        voltage_mV, currents_uA[0], currents_uA[1], 
        0, 0, 0
      );
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

int handlePointEIS(AFE *pOpenafeInstance, command_t *commandParams){
  char cmdBuf[128];
  size_t cmdIdx = 0;

  do {
    if (pOpenafeInstance->dataAvailable_EIS() > 0) {
      noInterrupts();
      float frequency_Hz;
      float impedance_real;
      float impedance_imag;
      uint8_t bCalibration;
      pOpenafeInstance->getPoint_EIS(&frequency_Hz, &impedance_real, &impedance_imag, &bCalibration);
      interrupts();        

      if(!bCalibration){
        _handlePointResult(
          commandParams->id, 
          0, 0, 0,
          frequency_Hz, impedance_real, impedance_imag
        );
      }
      
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
  } while (!pOpenafeInstance->doneEIS()); 

  Serial.flush();
  delay(10);// GArantir que todos os dados serão enviados pela serial

  detachInterrupt(digitalPinToInterrupt(2));
  return EXE_CVW_DONE;
}