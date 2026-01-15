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

static int handlePointUnified(AFE *pOpenafeInstance, command_t *commandParams, bool isEIS) {
  char cmdBuf[128];
  size_t cmdIdx = 0;

  do {
    if (isEIS) {
      if (pOpenafeInstance->dataAvailable_EIS() > 0) {
        noInterrupts();
        float frequency_Hz;
        float impedance_real;
        float impedance_imag;
        uint8_t bCalibration;
        pOpenafeInstance->getPoint_EIS(&frequency_Hz, &impedance_real, &impedance_imag, &bCalibration);
        interrupts();

        if (!bCalibration) {
          _handlePointResult(
            commandParams->id,
            0.0f, 0.0f, 0.0f,
            frequency_Hz, impedance_real, impedance_imag
          );
        }
      }
    } else {
      if (pOpenafeInstance->dataAvailable() > 0) {
        noInterrupts();
        float voltage_mV;
        float currents_uA[2];
        pOpenafeInstance->getPoint(&voltage_mV, currents_uA);
        interrupts();

        _handlePointResult(
          commandParams->id,
          voltage_mV, currents_uA[0], currents_uA[1],
          0.0f, 0.0f, 0.0f
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
              sendStatus((status_id_t)tInterpretResult);
            } else {
              int tExecutionResult = openAFEExecutioner_executeCommand(pOpenafeInstance, commandParams);
              if (tExecutionResult < 0) sendStatus((status_id_t)tExecutionResult);
            }
          } else {
            sendStatus((status_id_t)ERROR_INVALID_COMMAND);
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
  } while ( isEIS ? !pOpenafeInstance->doneEIS() : !pOpenafeInstance->done() );

  Serial.flush();
  delay(10);
  detachInterrupt(digitalPinToInterrupt(2));

  return EXE_CVW_DONE;
}

int handlePoint(AFE *pOpenafeInstance, command_t *commandParams){
  return handlePointUnified(pOpenafeInstance, commandParams, false);
}

int handlePointEIS(AFE *pOpenafeInstance, command_t *commandParams){
  return handlePointUnified(pOpenafeInstance, commandParams, true);
}
