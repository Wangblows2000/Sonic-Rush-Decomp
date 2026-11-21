#include <nitro/types.h>

u8 *ResultsScreen_GetPartnerRecord(void);

#pragma thumb on
BOOL ResultsScreen_HasPartnerDecided(void) {
    if (ResultsScreen_GetPartnerRecord()[1] != 0)
        return TRUE;
    return FALSE;
}
#pragma thumb reset
