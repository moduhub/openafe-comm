#include "../openAFE_Interpreter.hpp"

void _parseParams(command_t *p, String cmd, uint8_t cmdId){
  String params[7];

  uint8_t n = 0;

  switch (cmdId){
    case CMDID_CVW: n = 6; break;
    case CMDID_DPV:
    case CMDID_SWV: n = 7; break;
    case CMDID_EIS: n = 4; break;
    default: return;
  }

  separateCommandParameters(params, cmd, ',', n);

  p->id = cmdId;
  p->settlingTime = params[0].toInt();

  switch (cmdId){
    case CMDID_CVW:
      p->startingPotential = params[1].toFloat();
      p->endingPotential   = params[2].toFloat();
      p->scanRate          = params[3].toFloat();
      p->stepPotential     = params[4].toFloat();
      p->numCycles         = params[5].toInt();
      break;

    case CMDID_DPV:
    case CMDID_SWV:
      p->startingPotential = params[1].toFloat();
      p->endingPotential   = params[2].toFloat();
      p->scanRate          = params[3].toFloat();
      p->stepPotential     = params[4].toFloat();
      p->pulsePotential    = params[5].toFloat();
      p->dutyCycle         = params[6].toFloat();
      break;

    case CMDID_EIS:
      p->startingOmega     = params[1].toInt();
      p->endingOmega       = params[2].toInt();
      p->stepForADecade    = params[3].toInt();
      break;
  }
}
