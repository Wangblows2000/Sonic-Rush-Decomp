/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_0224ff44[];
extern u8 data_022b4574[];
void func_0204138c();

void func_020426e8(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = *(u32 *)data_022b4574;
    *(u16 *)((u8 *)t0 + 0x14) = *(u16 *)*(u32 *)data_0224ff44;
    s32 t2 = *(u16 *)((u8 *)t0 + 0x1a);
    s32 v3 = t1;
    if (t2 == 0xf0) {
        *(u16 *)((u8 *)t0 + 0x1a) = (t2 + 0x1);
        *(u32 *)((u8 *)t0 + 0xc) = 3;
        *(u32 *)t0 = (u32)func_0204138c;
        v3 = 3;
    } else {
        if ((u32)t2 < (u32)0xf0) {
            *(u16 *)((u8 *)t0 + 0x1a) = (t2 + 0x1);
        }
    }
    ((s32 (*)())*(u32 *)t0)(t0, *(u32 *)t0, v3);
}
