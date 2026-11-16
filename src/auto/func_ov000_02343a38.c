/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
extern u8 data_022503e0[];
s32 func_ov000_0234d5b8();

void func_ov000_02343a38(void) {
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x32c) & 0x200000) != 0) {
        return;
    }
    if (*(u8 *)((u8 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x325) != 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = func_ov000_0234d5b8(*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10), *(u16 *)data_022503e0);
    *(u16 *)((u8 *)(t0 + 0x400) + 0x20) = t1;
    s32 t2 = func_ov000_0234d5b8(t0, *(u16 *)((u8 *)data_022503e0 + 0x4));
    *(u16 *)((u8 *)(t0 + 0x400) + 0x22) = t2;
    s32 t3 = func_ov000_0234d5b8(t0, *(u16 *)((u8 *)data_022503e0 + 0x8));
    *(u16 *)((u8 *)(t0 + 0x400) + 0x24) = t3;
}
