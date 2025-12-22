#ifndef _OPENAFE_SERIAL_
#define _OPENAFE_SERIAL_

#include "Arduino.h"

#include "../util/CRC.hpp"
#include "../openAFEComm_Shared.hpp"

/**
 * @brief Sends the proper error message accorrding to the error code passed.
 *
 * @param pErrorCode IN -- Code of the error ocurred, e.g.: -1 (ERROR_INVALID_COMMAND).
 */
void sendError(int pErrorCode);

/** 
 * @brief Sends the given message to the App, adding the suffix and CRC.
 * 
 * @note Note that the message shall not include the suffix ($) nor 
 * asterix (*), e.g.: "MSG,RDY".
 * 
 * @param pMessage IN -- Raw message to be sent, e.g.: "MSG,RDY".
 */
void sendMessage(String pMessage);

/**
 * @brief Send a message to the App aknowledging that the command was received.
 * 
 */
void send_msg_received(void);

/**
 * @brief Send message to the App that the device is ready.
 * 
 */
void send_ready(void);

/**
 * @brief Send message to the App that the wave generation is starting.
 * 
 */
void send_msg_startingVoltammetry(void);

/**
 * @brief Send message to the App that the wave generation is finished.
 *
 */
void send_msg_endOfVoltammetry(void);

/**
 * @brief Send message to the App that the spectroscopy is starting.
 *
 */
void send_msg_startingSpectroscoy(void);

/**
 * @brief Send message to the App that the spectroscopy is finished.
 *
 */
void send_msg_endOfSpectroscopy(void);

/**
 * @brief send a command to serial, indicating that the CUR has been modified successfully
 *
 */
void send_msg_CURUpdate(void);

/**
 * @brief Send a command to serial, indicating that the TIA has been modified successfully
 *
 */
void send_msg_TIAUpdate(void);

/**
 * @brief Sends a CV point via serial
 *
 * @param pVoltage_mV - Voltage in mV
 * @param pCurrent_uA - Current in uA
 */
void sendPointCV(float pVoltage_mV, float pCurrent_uA);

/**
 * @brief Sends a DPV point via serial
 *
 * @param pVoltage_mV - Voltage in mV
 * @param pCurrent_uA - Current in uA
 * @param pCurrent_uA_2 - Second Current in uA
 */
void sendPointDPV(float pVoltage_mV, float pCurrent_uA, float pCurrent_uA_2);

/**
 * @brief Sends a SW point via serial
 *
 * @param pVoltage_mV - Voltage in mV
 * @param pCurrent_uA - Current in uA
 * @param pCurrent_uA_2 - Second Current in uA
 */
void sendPointSW(float pVoltage_mV, float pCurrent_uA, float pCurrent_uA_2);

/**
 * @brief Sends an EIS point via serial
 *
 * @param frequency_Hz - Frequency in Hz
 * @param impedance_real - Real part of impedance
 * @param impedance_imag - Imaginary part of impedance
 */
void sendPointEIS(float frequency_Hz, float impedance_real, float impedance_imag);

#endif //_OPENAFE_SERIAL_