#include "../openAFE_interpreter.hpp"

uint8_t commandToId(const String& pCommandString) {
  if (pCommandString.length() < 4) return 0; // segurança

  const char* cmd = pCommandString.c_str();

  // Example: $CMD,RUN*81 -> gets CMD
  if (strncmp(cmd + 1, "CVW", 3) == 0) return CMDID_CVW;
  else if (strncmp(cmd + 1, "DPV", 3) == 0) return CMDID_DPV;
  else if (strncmp(cmd + 1, "SWV", 3) == 0) return CMDID_SWV;
  else if (strncmp(cmd + 1, "EIS", 3) == 0) return CMDID_EIS;
  else if (strncmp(cmd + 1, "CMD", 3) == 0) {
    // Example: $CMD,RUN*81 -> gets the RUN
    if (pCommandString.length() < 8) return 0;
    if (strncmp(cmd + 5, "RUN", 3) == 0) return CMDID_RUN;
    else if (strncmp(cmd + 5, "CHK", 3) == 0) return CMDID_CHK;
    else if (strncmp(cmd + 5, "DIE", 3) == 0) return CMDID_DIE;
    else if (strncmp(cmd + 5, "TIA", 3) == 0) return CMDID_TIA;
    else if (strncmp(cmd + 5, "CUR", 3) == 0) return CMDID_CUR;
    else return 0;
  }
  else return 0;
}