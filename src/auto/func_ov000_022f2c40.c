/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022f2bf4();

void func_ov000_022f2c40(u8 *p0) {
    s32 t0 = *(u16 *)((u8 *)p0 + 0x45e);
    if (t0 != 0xc000) {
        *(u16 *)((u8 *)p0 + 0x45e) = (t0 - 0x200);
    }
    *(u16 *)((u8 *)p0 + 0x2e) = (*(s16 *)((u8 *)p0 + 0x2e) + 0x80);
    s32 t1 = *(s16 *)((u8 *)p0 + 0x2e);
    if (t1 > 0x1000) {
        *(u16 *)((u8 *)p0 + 0x2e) = 0x1000;
    }
    s32 t2 = *(u32 *)((u8 *)p0 + 0x10);
    if (t2 >= 0x5e800) {
        *(u16 *)((u8 *)p0 + 0x2e) = 0;
        *(u32 *)((u8 *)p0 + 0x10) = 0x5e800;
    }
    *(u16 *)((u8 *)p0 + 0x32) = (*(s16 *)((u8 *)p0 + 0x32) + 0x80);
    s32 t3 = *(s16 *)((u8 *)p0 + 0x32);
    if (t3 > 0x1000) {
        *(u16 *)((u8 *)p0 + 0x32) = 0x1000;
    }
    s32 t4 = *(u32 *)((u8 *)p0 + 0x18);
    if (t4 >= 0) {
        *(u16 *)((u8 *)p0 + 0x32) = 0;
        *(u32 *)((u8 *)p0 + 0x18) = 0;
    }
    if (*(u32 *)((u8 *)p0 + 0x10) != 0x5e800) {
        return;
    }
    if (*(u32 *)((u8 *)p0 + 0x18) != 0) {
        return;
    }
    s32 t5 = *(u16 *)((u8 *)p0 + 0x45e);
    if (t5 == 0xc000) {
        *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f2bf4;
    }
}
