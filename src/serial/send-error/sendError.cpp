#include "../openAFE_Serial.hpp"

void sendError(int pErrorCode){
	switch (pErrorCode){
    case ERROR_INVALID_COMMAND:
      sendMessage("ERR,INV");
      break;
    case ERROR_PARAM_OUT_BOUNDS:
      sendMessage("ERR,PAR");
      break;
    case ERROR_PARAM_MISSING:
      sendMessage("ERR,PAR");
      break;
    case ERROR_GENERAL:
      sendMessage("ERR,GEN");
      break;
    case ERROR_AFE_NOT_WORKING:
      sendMessage("ERR,AFE");
      break;
    case ERROR_WAVE_GEN:
      sendMessage("ERR,WAV");
    default:
      break;
	}
}