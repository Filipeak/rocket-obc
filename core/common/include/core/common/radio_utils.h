#pragma once

#include <obc/api.h>
#include "drivers/lora/sx127X_driver.h"
#include "drivers/lora/radio_protocol.h"

#define MEASUREMENTS_RADIO_COMMAND_ID 'M'

/**
 * @brief Radio packet wrapper structure
 */
typedef struct RadioUtilPacketData
{
    RadioBody body;       /** Packet body */
    INT32 signalStrength; /** RSSI.*/
} RadioUtilPacketData;

/**
 * @brief Initialize radio module
 *
 * @param pinout Radio pinout
 */
VOID initializeRadio(SX127XPinout *pinout);

/**
 * @brief Check if radio packet is available
 *
 * @param packet Radio packet is available. Remeber to call radioClearPacket() on packet body after processing.
 * @return TRUE if radio packet is available, FALSE otherwise
 */
BOOL checkRadioPacket(RadioUtilPacketData *packet);

/**
 * @brief Send radio packet
 *
 * @param body Radio packet body
 */
VOID sendRadioPacket(RadioBody *body);