#include <avr/pgmspace.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../openAFE_Serial.hpp"

// Helper: converte float para string (dtostrf), elimina espaços à esquerda e concatena no buffer destino */
static void appendFloatTrim(char *dest, float val, uint8_t prec){
  char tmp[20];
  dtostrf(val, 0, prec, tmp);

  char *p = tmp;
  while (*p == ' ') p++;

  strcat(dest, p);
}

/* sendPoint unificado:
   - PT_EIS: 3 valores (frequency, real, imag)
   - PT_CV:  2 valores (voltage, current)
   - PT_DPV/PT_SWV: 3 valores (voltage, current, current2)
*/
void sendPoint(point_type_t type, float v1, float v2, float v3){
  char buffer[64];
  buffer[0] = '\0';

  if (type == PT_EIS) {
    strcpy_P(buffer, pr_eis);    // "EOT,"
    appendFloatTrim(buffer, v1, 2);
    strcat(buffer, ",");
    appendFloatTrim(buffer, v2, 3);
    strcat(buffer, ",");
    appendFloatTrim(buffer, v3, 3);
  } else {
    strcpy_P(buffer, pr_sgl);    // "SGL,"
    appendFloatTrim(buffer, v1, 3);
    strcat(buffer, ",");
    appendFloatTrim(buffer, v2, 3);
    if (type == PT_DPV || type == PT_SWV) { // DPV/SWV
      strcat(buffer, ",");
      appendFloatTrim(buffer, v3, 3);
    }
  }

  sendMessage(buffer);
}