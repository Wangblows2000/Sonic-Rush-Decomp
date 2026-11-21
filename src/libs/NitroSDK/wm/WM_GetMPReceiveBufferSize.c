#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);

s32 WM_GetMPReceiveBufferSize(void)
{
    UnkWmWork *w = WMi_GetSystemWork();
    u8 *p;
    BOOL isParent;
    u16 size;
    if (WMi_CheckStateEx(2, 7, 8) != 0) {
        return 0;
    }
    DC_InvalidateRange(w->unk4 + 0xc, 4);
    p = w->unk4;
    if (*(u32 *)(p + 0xc) == 1) {
        return 0;
    }
    DC_InvalidateRange(p + 0x184, 2);
    isParent = (*(u16 *)(w->unk4 + 0x184) == 0) ? TRUE : FALSE;
    DC_InvalidateRange(w->unk4 + 0x190, 2);
    size = *(u16 *)(w->unk4 + 0x190);
    if (isParent != TRUE) {
        return ((size + 0x51) & ~0x1f) * 2;
    }
    DC_InvalidateRange(w->unk4 + 0xf4, 2);
    return (((size + 0xc) * *(u16 *)(w->unk4 + 0xf4) + 0x29) & ~0x1f) * 2;
}
