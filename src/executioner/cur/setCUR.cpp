#include "../openAFE_Executioner.hpp"

int _setCurrentRange(AFE *pOpenafeInstance, command_t *pCommandParams){
	if (pOpenafeInstance)
	{
		pOpenafeInstance->setCurrentRange(pCommandParams->currentRange);
		return CMDID_CUR_SETTED;
	} else {
		return ERROR_GENERAL;
	}
}