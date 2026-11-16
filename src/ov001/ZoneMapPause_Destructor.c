#include <nitro/types.h>

extern void *gZoneMapPauseWork;
void func_ov001_022e9308(void *p);
void func_ov001_022e9304(void *p);
void ZoneMapPause_ResetDisplay(void);

#pragma thumb on
void ZoneMapPause_Destructor(u8 *p) {
    void *q = *(void **)(p + 0x10);
    func_ov001_022e9308(q);
    func_ov001_022e9304(q);
    ZoneMapPause_ResetDisplay();
    if (q == gZoneMapPauseWork) {
        gZoneMapPauseWork = NULL;
    }
}
#pragma thumb reset
