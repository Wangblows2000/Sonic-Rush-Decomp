#include <nitro/types.h>

u8 *ResultsScreen_GetPartnerRecord(void);

#pragma thumb on
u8 ResultsScreen_GetPartnerChoice(void) {
    return *ResultsScreen_GetPartnerRecord();
}
#pragma thumb reset
