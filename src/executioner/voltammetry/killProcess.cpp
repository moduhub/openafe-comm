#include "../openAFE_Executioner.hpp"

int _killProcess(AFE *pOpenafeInstance){
	if (pOpenafeInstance) pOpenafeInstance->killVoltammetry();
  delay(100);
  asm volatile ("  jmp 0");
	return 1;
}