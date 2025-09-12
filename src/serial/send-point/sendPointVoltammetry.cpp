#include "../openAFE_Serial.hpp"

void sendPointCV(float pVoltage_mV, float pCurrent_uA){
  String tSinglePointResult = "SGL," + String(pVoltage_mV) + "," + String(pCurrent_uA);
  sendMessage(tSinglePointResult);
}

void sendPointDPV(float pVoltage_mV, float pCurrent_uA, float pCurrent_uA_2){
  String tSinglePointResult = "SGL," + String(pVoltage_mV) + "," + String(pCurrent_uA) + "," + String(pCurrent_uA_2);
  sendMessage(tSinglePointResult);
}

void sendPointSW(float pVoltage_mV, float pCurrent_uA, float pCurrent_uA_2){
  String tSinglePointResult = "SGL," + String(pVoltage_mV) + "," + String(pCurrent_uA) + "," + String(pCurrent_uA_2);
  sendMessage(tSinglePointResult);
}