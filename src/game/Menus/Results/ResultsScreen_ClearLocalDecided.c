#include <nitro/types.h>

u8 *ResultsScreen_GetLocalRecord(void *p);

#pragma thumb on
void ResultsScreen_ClearLocalDecided(void *p) {
    ResultsScreen_GetLocalRecord(p)[1] = 0;
}
#pragma thumb reset
