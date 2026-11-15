#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);

s32 func_02011400(void)
{
    UnkWmWork *w = func_02010b70();
    u8 *p;
    if (func_02010a50(2, 7, 8) != 0) {
        return 0;
    }
    DC_InvalidateRange(w->unk4 + 0xc, 4);
    p = w->unk4;
    if (*(u32 *)(p + 0xc) == 1) {
        return 0;
    }
    DC_InvalidateRange(p + 0x18e, 4);
    return (*(u16 *)(w->unk4 + 0x18e) + 0x1f) & ~0x1f;
}
