#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "config.h"
#include "radio_controller.h"
#include "receiver_logger.h"
#include "shared/commands_utils.h"
#include "shared/radio_utils.h"
#include "drivers/console/console_output.h"
#include "kernel/logging/logger.h"

static RadioUtilPacketData s_Packet;

int main()
{
    stdio_init_all();

    sleep_ms(7000);

    myLogCreateConsoleSink(myLogGetCoreLogger(), DEFAULT_LOG_SERIAL_PATTERN);

    MY_LOG_CORE_INFO("Initializing...");

    SX1278Pinout loraPinout = getRadioPinout();

    initializeCommands();
    initializeRadio(&loraPinout);
    initializeReceiverLogger();

    MY_LOG_CORE_INFO("Everything is ready!");

    while (TRUE)
    {
        if (consoleAvailable())
        {
            checkCommand();
        }

        if (checkRadioPacket(&s_Packet))
        {
            if (s_Packet.body.command == MEASUREMENTS_RADIO_COMMAND_ID)
            {
                MeasurementData measurement = {0};

                memcpy(&measurement, &s_Packet.body.payload, s_Packet.body.payloadSize);

                ReceiverSendData data = {
                    .measurement = measurement,
                };

                logReceiverData(&data);
            }
            else if (s_Packet.body.command == COMMANDS_RADIO_COMMAND_ID)
            {
                radioRemoteCommandCallback(s_Packet.body.payload, s_Packet.body.payloadSize);
            }

            radioClearPacket(&s_Packet.body);
        }
    }

    return 0;
}