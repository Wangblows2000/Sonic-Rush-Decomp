#include <nitro/types.h>

u8 *ResultsScreen_GetPartnerRecord(void);

#pragma thumb on
BOOL ResultsScreen_HasPartnerQuit(void) {
    if (ResultsScreen_GetPartnerRecord()[2] != 0)
        return TRUE;
    return FALSE;
}
#pragma thumb reset
