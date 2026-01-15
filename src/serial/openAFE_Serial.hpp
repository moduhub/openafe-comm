#ifndef _OPENAFE_SERIAL_
#define _OPENAFE_SERIAL_

#include "Arduino.h"

#include "../util/CRC.hpp"
#include "../openAFEComm_Shared.hpp"

/** 
 * @brief Sends the given message to the App, adding the suffix and CRC.
 * 
 * @note Note that the message shall not include the suffix ($) nor 
 * asterix (*), e.g.: "MSG,RDY".
 * 
 * @param pMessage IN -- Raw message to be sent, e.g.: "MSG,RDY".
 */
void sendMessage(const char *msg);

/**
 * @brief Status identifiers for communication between the AFE and the host application.
 * 
 * This enum defines both error codes and status messages that can be sent to the host.
 */
typedef enum {
  // Errors
  ST_ERR_INV,
  ST_ERR_PAR,
  ST_ERR_GEN,
  ST_ERR_AFE,
  ST_ERR_WAV,

  // Messages
  ST_MSG_CUR_UPDT,
  ST_MSG_EIS_START,
  ST_MSG_END,
  ST_MSG_RECEIVED,
  ST_MSG_READY,
  ST_MSG_TIA_UPDT,
  ST_MSG_VOLTAMMETRY_START,

  ST_COUNT
} status_id_t;

/** 
 * @brief Error and message strings stored in program memory (PROGMEM). 
 */
static const char st_err_inv[] PROGMEM = "ERR,INV";
static const char st_err_par[] PROGMEM = "ERR,PAR";
static const char st_err_gen[] PROGMEM = "ERR,GEN";
static const char st_err_afe[] PROGMEM = "ERR,AFE";
static const char st_err_wav[] PROGMEM = "ERR,WAV";

static const char st_msg_cur_updt[] PROGMEM = "MSG,CUR,UPDT";
static const char st_msg_eis_s[]    PROGMEM = "MSG,EISS";
static const char st_msg_end[]      PROGMEM = "MSG,END";
static const char st_msg_rcd[]      PROGMEM = "MSG,RCD";
static const char st_msg_rdy[]      PROGMEM = "MSG,RDY";
static const char st_msg_tia_updt[] PROGMEM = "MSG,TIA,UPDT";
static const char st_msg_cvs[]      PROGMEM = "MSG,CVS";

/**
 * @brief Lookup table mapping status identifiers to their string representation.
 * 
 * This table is stored in program memory (PROGMEM) to save RAM usage.
 */
static const char* const statusTable[ST_COUNT] PROGMEM = {
  // Errors
  st_err_inv,
  st_err_par,
  st_err_gen,
  st_err_afe,
  st_err_wav,

  // Messages
  st_msg_cur_updt,
  st_msg_eis_s,
  st_msg_end,
  st_msg_rcd,
  st_msg_rdy,
  st_msg_tia_updt,
  st_msg_cvs
};

/**
 * @brief Sends a status message to the App.
 * 
 * This function retrieves the corresponding string from the status table
 * and sends it using `sendMessage()`.
 * 
 * @param id IN -- Status identifier (error or message).
 */
void sendStatus(status_id_t id);

// Prefixes for point messages
static const char pr_eis[] PROGMEM = "EOT,";
static const char pr_sgl[] PROGMEM = "SGL,";

/**
 * @brief Point types used in measurement processes.
 * 
 * Defines the type of point data being sent to the host application.
 */
typedef enum {
  PT_EIS,
  PT_CV,
  PT_DPV,
  PT_SWV
} point_type_t;

/**
 * @brief Sends a measurement point to the App.
 * 
 * Depending on the point type, this function formats the data accordingly:
 * - **PT_EIS**: frequency, real impedance, imaginary impedance.
 * - **PT_CV**: voltage, current.
 * - **PT_DPV/PT_SWV**: voltage, current1, current2.
 * 
 * @param type IN -- Point type (EIS, CV, DPV, SWV).
 * @param v1 IN -- First value (frequency or voltage).
 * @param v2 IN -- Second value (current or real impedance).
 * @param v3 IN -- Third value (optional: current2 or imaginary impedance).
 */
void sendPoint(point_type_t type, float v1, float v2, float v3);

#endif //_OPENAFE_SERIAL_