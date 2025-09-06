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
 * @brief Send message to the App that the wave generation is finished.
 *
 */
void send_endOfSpectroscopy(void);