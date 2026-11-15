#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void MIi_CpuCopyFast(const void *src, void *dst, u32 size);

s32 func_02011484(void *dst)
{
    UnkWmWork *w = func_02010b70();
    s32 r = func_02010b54();
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
