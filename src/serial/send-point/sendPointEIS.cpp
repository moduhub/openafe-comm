#include "../openAFE_Serial.hpp"

void sendPointEIS(float frequency_Hz, float impedance_real, float impedance_imag){
  String tSinglePointResult = "EOT," + String(frequency_Hz) + "," + String(impedance_real) + "," + String(impedance_imag);
  sendMessage(tSinglePointResult);
}
