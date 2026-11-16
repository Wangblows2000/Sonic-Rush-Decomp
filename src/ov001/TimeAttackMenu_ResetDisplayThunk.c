#include <nitro/types.h>

void TimeAttackMenu_ResetDisplay(void);

#pragma thumb on
void TimeAttackMenu_ResetDisplayThunk(void) {
    TimeAttackMenu_ResetDisplay();
}
#pragma thumb reset
