#include <nitro/types.h>

u8 *ResultsScreen_GetLocalRecord(void *p);

#pragma thumb on
void ResultsScreen_SetLocalDecided(void *p) {
    ResultsScreen_GetLocalRecord(p)[1] = 1;
}
#pragma thumb reset
