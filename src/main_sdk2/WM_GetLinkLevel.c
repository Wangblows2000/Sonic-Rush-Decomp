#include <nitro/types.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);

u16 WM_GetLinkLevel(void)
{
    UnkWmWork *w = func_02010b70();
    if (func_02010b54() != 0) {
        return 0;
    }
    DC_InvalidateRange(w->unk4, 2);
    switch (*(u16 *)w->unk4) {
    case 9:
        DC_InvalidateRange(w->unk4 + 0x17e, 2);
        if (*(u16 *)(w->unk4 + 0x17e) == 0) {
            return 0;
        }
    case 10:
    case 11:
        DC_InvalidateRange(w->unk4 + 0xb8, 2);
        return *(u16 *)(w->unk4 + 0xb8);
    default:
        return 0;
    }
}
