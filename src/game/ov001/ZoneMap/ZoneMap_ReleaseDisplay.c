#include <nitro/types.h>

void ZoneMap_ResetDisplay(void);
void ZoneMap_EnableBlend(BOOL on);

#pragma thumb on
void ZoneMap_ReleaseDisplay(void) {
    ZoneMap_ResetDisplay();
    ZoneMap_EnableBlend(FALSE);
}
#pragma thumb reset
