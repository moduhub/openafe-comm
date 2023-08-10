#ifndef _OPENAFE_ERROR_CODES_
#define _OPENAFE_ERROR_CODES_

#define ERROR_INVALID_COMMAND -1  // Command is invalid.
#define ERROR_PARAM_OUT_BOUNDS -2 // One or more parameters are out of bounds.
#define ERROR_PARAM_MISSING -3    // One or more parameters are missing.
#define ERROR_GENERAL -4          // A general error ocorred, an error that does not fit in other cathegories.
#define ERROR_AFE_NOT_WORKING -5  // AFE IC does not respond as expected.
#define ERROR_WAVE_GEN -6         // An error ocurred during the wave generation.

#endif //_OPENAFE_ERROR_CODES_