#ifndef _OPENAFE_EXECUTIONER_
#define _OPENAFE_EXECUTIONER_

#include "openafe.h"
#include "../openAFEComm_Shared.hpp"
#include "../interpreter/openAFE_Interpreter.hpp"
#include "../serial/openAFE_Serial.hpp"
#include "../util/CRC.hpp"
#include "Arduino.h"

/**
 * @brief Set the callback to pass the point result up. The callback function should be of void 
 * type and receive two float parameters, one being the voltage in millivolts (mV) and the other 
 * the current in microAmps (uA), e.g.: void handlePoint(float pVoltage_mV, float pVoltage_uA).
 *
 * @param pPointResultMessageCallback IN -- Callback to the function to send the SGL point message.
 */
void openAFEExecutioner_setPointResultMessageCallback(void (*pPointResultMessageCallback)(
  int, 
  float, float, float,
  float, float, float
));

/**
 * @brief Execute a received command with parameters.
 * 
 * @param pCommandParams IN -- Command struct with parameters.
 * @return >0 If successful, error code on error. 
 */
int openAFEExecutioner_executeCommand(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief
 */
int handlePoint(AFE *pOpenafeInstance, command_t *commandParams);

/**
 * @brief Handle incoming point results during an EIS run.
 *
 * This function drives the loop that reads points from the AFE in impedance
 * spectroscopy mode and dispatches them through the registered point callback.
 *
 * @param pOpenafeInstance IN -- OpenAFE class instance.
 * @param commandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int handlePointEIS(AFE *pOpenafeInstance, command_t *commandParams);

/**
 * @brief Check the value in the AFE IC ADIID register. If the AFE IC does
 * not give the expected 0x4144 then something is wrong. 
 * 
 * @return >0 if successful, error code on error. 
 */
int _checkAFEHealth(AFE *pOpenafeInstance);

/**
 * @brief Stop the currently undergoing voltammetry.
 * 
 * @param pOpenafeInstance IN -- OpenAFE class instance.
 * @return 
 */
int _killProcess(AFE *pOpenafeInstance);

/**
 * @brief Handle the received point result from voltammetry proccess.
 * 
 * @param cmdId IN -- Command ID of the ongoing process (CMDID_CVW, CMDID_DPV, CMDID_SWV)
 * 
 */
void _handlePointResult(
  int cmdId, 
  float voltage, float current_1, float current_2,
  float frequency, float impedance_real, float impedance_imag
);

/**
 * @brief Executes the Cyclic Voltammetry with the received parameters.
 * The function will return error code in case of any error. 
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _executeCyclicVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief Executes the Differential Pulse Voltammetry with the received parameters.
 * The function will return error code in case of any error. 
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _executeDifferentialPulseVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief Executes the Square Wave Voltammetry with the received parameters.
 * The function will return error code in case of any error. 
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _executeSquareWaveVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief Executes the EIS with the received parameters.
 * The function will return error code in case of any error. 
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _executeImpedanceSpectroscopy(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief Set the TIA Gain Resistor.
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _setTIAGainResistor(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief Set the current range of the AFE.
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _setCurrentRange(AFE *pOpenafeInstance, command_t *pCommandParams);

#endif //_OPENAFE_EXECUTIONER_