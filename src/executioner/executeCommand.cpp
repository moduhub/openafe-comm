#include "openAFE_Executioner.hpp"

int openAFEExecutioner_executeCommand(AFE *pOpenafeInstance, command_t *pCommandParams){

	switch (pCommandParams->id){
	case CMDID_CHK:
		return _checkAFEHealth(pOpenafeInstance);
		break;
	
	case CMDID_DIE:
		return _killProcess(pOpenafeInstance);
		break;

	case CMDID_TIA:
		return _setTIAGainResistor(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_CVW:
		return _executeCyclicVoltammetry(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_DPV:
		return _executeDifferentialPulseVoltammetry(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_SWV:
		return _executeSquareWaveVoltammetry(pOpenafeInstance, pCommandParams);
		break;

  case CMDID_EIS:
		return _executeImpedanceSpectroscopy(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_CUR:
		return _setCurrentRange(pOpenafeInstance, pCommandParams);
		break;

	default:
		return ERROR_INVALID_COMMAND;
		break;
	}
}