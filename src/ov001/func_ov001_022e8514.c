#include <nitro/types.h>

void func_ov001_022e85c8(void);
void ZoneMap_EnableBlend(BOOL on);

#pragma thumb on
void func_ov001_022e8514(void) {
    func_ov001_022e85c8();
    ZoneMap_EnableBlend(FALSE);
}
#pragma thumb reset
