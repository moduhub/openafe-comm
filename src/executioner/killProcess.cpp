#include "openAFE_Executioner.hpp"

int _killProcess(AFE *pOpenafeInstance){
	if (pOpenafeInstance) pOpenafeInstance->killProcess();
	return 1;
}