#include <nitro/types.h>
#include <NitroSDK/wmsys.h>

s32 WMi_CheckInitialized(void);
void DC_InvalidateRange(void *p, u32 size);

s32 WMi_CheckIdle(void)
{
    s32 r = WMi_CheckInitialized();
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(data_0207faec->state, 2);
    if (*data_0207faec->state <= 1) {
        return 3;
    }
    return 0;
}
