#include <nitro/types.h>
#include <main_sdk2/wmsys.h>

s32 func_02010b54(void);
void DC_InvalidateRange(void *p, u32 size);

s32 WMi_CheckIdle(void)
{
    s32 r = func_02010b54();
    if (r != 0) {
        return r;
    }
    DC_InvalidateRange(data_0207faec->state, 2);
    if (*data_0207faec->state <= 1) {
        return 3;
    }
    return 0;
}
