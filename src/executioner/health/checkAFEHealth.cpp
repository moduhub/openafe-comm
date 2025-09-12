#include "../openAFE_Executioner.hpp"

int _checkAFEHealth(AFE *pOpenafeInstance){
	if(pOpenafeInstance)
	{
		if (pOpenafeInstance->isAFEResponding()) {
			return 1;
		} else {
			return ERROR_AFE_NOT_WORKING;
		}
	} else {
		return ERROR_AFE_NOT_WORKING;
	}
	return ERROR_AFE_NOT_WORKING;
}