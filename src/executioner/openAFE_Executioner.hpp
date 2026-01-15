#ifndef _OPENAFE_EXECUTIONER_
#define _OPENAFE_EXECUTIONER_

#include "openafe.h"
#include "../openAFEComm_Shared.hpp"
#include "../interpreter/openAFE_Interpreter.hpp"
#include "../serial/openAFE_Serial.hpp"
#include "../util/CRC.hpp"
#include "Arduino.h"

/**
 * @brief Set the callback to pass the point result up. 
 * 
 * The callback function should be of type `void` and receive the following parameters:
 * - int cmdId: Command ID of the ongoing process.
 * - float voltage: Voltage in millivolts (mV).
 * - float current_1: First current measurement in microAmps (uA).
 * - float current_2: Second current measurement in microAmps (uA).
 * - float frequency: Frequency in Hz (used in EIS).
 * - float impedance_real: Real part of impedance (Ohms).
 * - float impedance_imag: Imaginary part of impedance (Ohms).
 *
 * @param pPointResultMessageCallback IN -- Callback to the function to send the point message.
 */
void openAFEExecutioner_setPointResultMessageCallback(void (*pPointResultMessageCallback)(
  int, 
  float, float, float,
  float, float, float
));

/**
 * @brief Execute a received command with parameters.
 * 
 * This function interprets and executes commands received from the host, 
 * such as voltammetry or impedance spectroscopy.
 *
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command struct with parameters.
 * @return >0 If successful, error code on error. 
 */
int openAFEExecutioner_executeCommand(AFE *pOpenafeInstance, command_t *pCommandParams);

/**
 * @brief Handle incoming point results during a voltammetry run.
 *
 * This function drives the loop that reads points from the AFE in voltammetry mode 
 * and dispatches them through the registered point callback.
 *
 * @param pOpenafeInstance IN -- OpenAFE class instance.
 * @param commandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
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
 * @param pOpenafeInstance IN -- OpenAFE class instance.
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
 * @brief Handle the received point result from measurement processes.
 * 
 * Dispatches the point result to the registered callback.
 *
 * @param cmdId IN -- Command ID of the ongoing process (CMDID_CVW, CMDID_DPV, CMDID_SWV, CMDID_EIS).
 * @param voltage IN -- Voltage in millivolts (mV).
 * @param current_1 IN -- First current measurement in microAmps (uA).
 * @param current_2 IN -- Second current measurement in microAmps (uA).
 * @param frequency IN -- Frequency in Hz (used in EIS).
 * @param impedance_real IN -- Real part of impedance (Ohms).
 * @param impedance_imag IN -- Imaginary part of impedance (Ohms).
 */
void _handlePointResult(
  int cmdId, 
  float voltage, float current_1, float current_2,
  float frequency, float impedance_real, float impedance_imag
);

/**
 * @brief Executes the Voltammetry with the received parameters.
 * The function will return error code in case of any error. 
 * 
 * @param pOpenafeInstance IN -- OpenAFE device instance.
 * @param pCommandParams IN -- Command parameters struct.
 * @return >0 if successful, error code on error.
 */
int _executeVoltammetry(AFE *pOpenafeInstance, command_t *pCommandParams);

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