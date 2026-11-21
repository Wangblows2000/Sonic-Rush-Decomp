#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void MIi_CpuCopyFast(const void *src, void *dst, u32 size);

s32 WM_ReadStatus(void *dst)
{
    UnkWmWork *w = WMi_GetSystemWork();
    s32 r = WMi_CheckInitialized();
    if (r != 0) {
        return r;
    }
    if (dst == NULL) {
        return 6;
    }
    DC_InvalidateRange(w->unk4, 0x7b8);
    MIi_CpuCopyFast(w->unk4, dst, 0x7b8);
    return 0;
}
