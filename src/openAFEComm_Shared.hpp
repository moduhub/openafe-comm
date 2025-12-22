#ifndef _OPENAFE_SHARED_
#define _OPENAFE_SHARED_

#include <stdint.h>

#define COMM_BUFFER_SIZE 64 // The size of the communication buffer, for commands and messages.

//#define CMDID_RST 0u   // Reset Hardware
#define CMDID_CHK 1u   // Check if the AFE IC is responding, AKA read the ADIID register.
#define CMDID_DIE 2u   // Kill any operation taking place in the MCU and reset it.
#define CMDID_RUN 3u   // Run the Voltammetry that is already loaded (for the sequencer ONLY).

#define CMDID_CVW 4u	// Feed CV parameters, check possibility of the wave and start the voltammetry.
#define CMDID_DPV 5u 	// Feed DPV parameters, check possibility of the wave and start the voltammetry.
#define CMDID_SWV 6u	// Feed SW parameters, check possibility of the wave and start the voltammetry.
#define CMDID_EIS 7u  // Feed EIS parameters, check possibility of the wave and start the Spectroscopy.

#define CMDID_TIA 10u // Command to change the Transimpedance Amplifier gain resistor.
#define CMDID_CUR 11u // Command to change the TIA gain resistor using the current as the parameter.
#define CMDID_TIA_SETTED 12u // Command to change the Transimpedance Amplifier gain resistor executed.
#define CMDID_CUR_SETTED 13u // Command to change the gain resistor of the TIA using the current as a parameter executed.

#define CMDID_CV_PARAMS 155u  // Feed the CV params and the sequencer (for the sequencer ONLY).
#define CMDID_DPV_PARAMS 156u // Feed the DVP params and the sequencer (for the sequencer ONLY).
#define CMDID_SW_PARAMS 157u  // Feed the SW params and the sequencer (for the sequencer ONLY).

#define EXE_CVW_DONE 200u // Execution of Cyclic voltammetry finished.
#define EXE_DPV_DONE 201u // Execution of Differential Pulse voltammetry finished.
#define EXE_SWV_DONE 202u // Execution of Square wave voltammetry finished.
#define EXE_EIS_DONE 203u // Execution of Impedance spectroscopy finished.

#define STATUS_VOLTAMMETRY_UNDERGOING  210u // The Voltammetry process is currently in progress.
#define STATUS_SPECTROSCOPY_UNDERGOING 211u // The Impedance process is currently in progress.

#define ERROR_INVALID_COMMAND -1  // Command is invalid.
#define ERROR_PARAM_OUT_BOUNDS -2 // One or more parameters are out of bounds.
#define ERROR_PARAM_MISSING -3    // One or more parameters are missing.
#define ERROR_GENERAL -4          // A general error ocorred, an error that does not fit in other cathegories.
#define ERROR_AFE_NOT_WORKING -5  // AFE IC does not respond as expected.
#define ERROR_WAVE_GEN -6         // An error ocurred during the wave generation.

// YOU MAY WANT TO TAKE A LOOK AT THIS:
// https://bitbucket.org/moduhub/pstat1fw-git/src/master/src/main.c
/**
 * @brief Command parameters type. Hold the information of a command.
 *
 */
struct command_t
{
	uint8_t id;				            // Command ID.
	uint16_t settlingTime;	      // Settling time, in milliseconds.
	float startingPotential;      // Starting potential, in mV.
	float endingPotential;	      // Ending potential, in mV.
  uint16_t startingOmega;       // Starting omega, in Hz.
  uint16_t endingOmega;         // Ending omega, in Hz.
  uint16_t stepForADecade;      // Number of steps per decade
	float scanRate;		       	    // Scan rate, in mV.
  uint16_t samplesPerFrequency; // Samples per Frequency
	float stepPotential;			    // Step size, in mV.
  float pulsePotential;         //
	uint8_t numCycles;		        // Number of cycles.
	uint16_t numPoints;		        // Number of points.
	uint16_t gainTIA;		          // TIA gain.
	uint16_t currentRange;	      // AFE current range, in uA.	
  float dutyCycle;              // 
};

#endif // _OPENAFE_SHARED_