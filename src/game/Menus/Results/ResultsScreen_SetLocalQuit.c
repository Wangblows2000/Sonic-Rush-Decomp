#include <nitro/types.h>

u8 *ResultsScreen_GetLocalRecord(void *p);

#pragma thumb on
void ResultsScreen_SetLocalQuit(void *p) {
    ResultsScreen_GetLocalRecord(p)[2] = 1;
}
#pragma thumb reset
