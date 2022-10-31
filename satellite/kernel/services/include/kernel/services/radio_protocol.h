#pragma once

#include "tools/typedefs.h"

#define RADIO_PACKET_SIGNATURE_LENGTH 16

/**
 * @brief Header of radio packet. (INTERNAL USE)
 */
typedef struct _RadioHeader
{
    /**
     * @brief Parity Size.
     */
    SIZE _paritySize;

    /**
     * @brief Parity.
     */
    BYTE *_parity;

    /**
     * @brief Radio packet signature.
     */
    BYTE _signature[RADIO_PACKET_SIGNATURE_LENGTH];
} _RadioHeader;

/**
 * @brief Body of radio packet.
 */
typedef struct RadioBody
{
    /**
     * @brief Packet command.
     */
    BYTE command;

    /**
     * @brief Padding to align struct. Don't use it.
     */
    BYTE _padding[3];

    /**
     * @brief Payload size.
     */
    SIZE payloadSize;

    /**
     * @brief Packet payload.
     */
    BYTE *payload;
} RadioBody;

/**
 * @brief Radio packet structure. (INTERNAL USE)
 */
typedef struct _RadioPacket
{
    /**
     * @brief Header.
     */
    _RadioHeader _header;

    /**
     * @brief Body.
     */
    RadioBody _body;
} _RadioPacket;

/**
 * @brief Serialize and encrypt radio packet.
 *
 * @param body_in Body of radio packet.
 * @param buffer_out_ptr Pointer to buffer.
 * @param size_out Size of buffer.
 *
 * @return True if success.
 */
BOOL serializeRadioPacket(RadioBody *body, BYTE **buffer_out_ptr, SIZE *size_out);

/**
 * @brief Deserialize and decrypt radio packet.
 *
 * @param buffer_in Buffer.
 * @param size_in Size of buffer.
 * @param body_out Body of radio packet.
 *
 * @return True if success.
 */
BOOL deserializeRadioPacket(BYTE *buffer, SIZE size, RadioBody *body_out);

/**
 * @brief Clears radio body. (i.e payload)
 *
 * @param body Radio body to clear
 */
VOID radioClearPacket(RadioBody *body);