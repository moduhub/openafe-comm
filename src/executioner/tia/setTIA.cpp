#include "../openAFE_Executioner.hpp"

int _setTIAGainResistor(AFE *pOpenafeInstance, command_t *pCommandParams){
	if (pOpenafeInstance)
	{
		pOpenafeInstance->setTIAGain(pCommandParams->gainTIA);
		return CMDID_TIA_SETTED;
	} else {
		return ERROR_GENERAL;
	}
}