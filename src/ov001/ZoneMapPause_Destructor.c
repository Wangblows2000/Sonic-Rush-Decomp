#include <nitro/types.h>

extern void *gZoneMapPauseWork;
void ZoneMapPause_ReleaseAssets(void *p);
void ZoneMapPause_Dummy(void *p);
void ZoneMapPause_ResetDisplay(void);

#pragma thumb on
void ZoneMapPause_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    ZoneMapPause_ReleaseAssets(q);
    ZoneMapPause_Dummy(q);
    ZoneMapPause_ResetDisplay();
    if (q == gZoneMapPauseWork) {
        gZoneMapPauseWork = NULL;
    }
}
#pragma thumb reset
