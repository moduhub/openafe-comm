#include <avr/pgmspace.h>
#include "../openAFE_Serial.hpp"


void sendStatus(status_id_t id){
  if (id >= ST_COUNT)
    return;

  char buffer[14];  // Larger string: "MSG,CUR,UPDT"
  strcpy_P(buffer, (PGM_P)pgm_read_ptr(&statusTable[id]));
  sendMessage(buffer);
}