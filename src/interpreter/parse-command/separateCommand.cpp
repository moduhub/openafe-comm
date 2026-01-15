#include "../openAFE_Interpreter.hpp"

void separateCommandParameters(String pParamArray[], String pCommandString, char pSeparator, uint8_t pNumParams) {
  int startPos = pCommandString.indexOf(',');
  int endPos = pCommandString.indexOf('*');
  
  if (startPos == -1 || endPos == -1) {
    Serial.println(F("Error: invalid command format"));
    return;
  }

  String paramsStr = pCommandString.substring(startPos + 1, endPos);
  
  for (uint8_t i = 0; i < pNumParams; i++) {
    pParamArray[i] = "";
  }

  uint8_t paramIndex = 0;
  int strLen = paramsStr.length();
  int lastComma = 0;
  
  for (int i = 0; i < strLen && paramIndex < pNumParams; i++) {
    if (paramsStr[i] == ',' || i == strLen - 1) {
      if (i == strLen - 1 && paramsStr[i] != ',') {
        pParamArray[paramIndex] = paramsStr.substring(lastComma, i + 1);
      } else {
        pParamArray[paramIndex] = paramsStr.substring(lastComma, i);
      }
      lastComma = i + 1;
      paramIndex++;
    }
  }

  if (paramIndex < pNumParams) {
    Serial.println(F("Warning: fewer parameters than expected"));
  }
}