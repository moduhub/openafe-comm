#include "openAFE_Executioner.hpp"

int openAFEExecutioner_executeCommand(AFE *pOpenafeInstance, command_t *pCommandParams){

	switch (pCommandParams->id){
	case CMDID_CHK:
		return;
    // [removed] Removed to increase available space for measurements
    // _checkAFEHealth(pOpenafeInstance);
		break;
	
	case CMDID_DIE:
		return _killProcess(pOpenafeInstance);
		break;

	case CMDID_TIA:
		return;
    // [removed] The memory of the Arduino R3 is limiting processes; the TIA and CUR settings were removed to gain more space.
    //  _setTIAGainResistor(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_CVW:
	case CMDID_DPV:
	case CMDID_SWV:
		return _executeVoltammetry(pOpenafeInstance, pCommandParams);
		break;

  case CMDID_EIS:
		return _executeImpedanceSpectroscopy(pOpenafeInstance, pCommandParams);
		break;

	case CMDID_CUR:
		return;
    // [removed] The memory of the Arduino R3 is limiting processes; the TIA and CUR settings were removed to gain more space.
    // _setCurrentRange(pOpenafeInstance, pCommandParams);
		break;

	default:
		return ERROR_INVALID_COMMAND;
		break;
	}
}