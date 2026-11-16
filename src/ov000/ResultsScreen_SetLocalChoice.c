#include <nitro/types.h>

u8 *ResultsScreen_GetLocalRecord(void *p);

#pragma thumb on
void ResultsScreen_SetLocalChoice(void *p, u8 v) {
    *ResultsScreen_GetLocalRecord(p) = v;
}
#pragma thumb reset
