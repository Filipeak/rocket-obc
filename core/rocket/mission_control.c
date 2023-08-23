#include "mission_control.h"
#include "measurements_utils.h"

static BOOL s_GPSInitialized;

BOOL isMissionReady(VOID)
{
    if (checkGPS() && !s_GPSInitialized)
    {
        s_GPSInitialized = TRUE;
    }

    if (!s_GPSInitialized)
    {
        return FALSE;
    }

    return TRUE;
}