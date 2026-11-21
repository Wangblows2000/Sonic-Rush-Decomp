#include <nitro/types.h>

void TimeSelectMenu_ResetBgOffsets(void);

#pragma thumb on
void TimeSelectMenu_ReleaseDisplay(void) {
    TimeSelectMenu_ResetBgOffsets();
}
#pragma thumb reset
