/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022dd9e8();
s32 func_020446b0();
extern u8 data_ov000_0236a444[];
s32 func_02044690();
s32 func_02044670();

void func_ov000_022ddab8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x224);
    if (t0 > 0x6e) {
        s32 t1 = *(u32 *)((u8 *)p0 + 0x820);
        s32 t2 = (*(u32 *)((u8 *)p0 + 0x820) * -1);
        *(u32 *)((u8 *)p0 + 0x820) = t2;
        *(u32 *)((u8 *)p0 + 0x220) = (u32)func_ov000_022dd9e8;
        *(u32 *)((u8 *)p0 + 0x224) = 0;
        func_020446b0(0, 0, t1, t2);
        *(u32 *)((u8 *)data_ov000_0236a444 + 0x3c) = 0;
        *(u32 *)((u8 *)data_ov000_0236a444 + 0x40) = 0;
        return;
    }
    s32 t3 = func_02044690(p0, t0);
    *(u32 *)((u8 *)data_ov000_0236a444 + 0x3c) = (t3 << 0xd);
    s32 t4 = func_02044670((t3 << 0xd), data_ov000_0236a444);
    *(u32 *)((u8 *)data_ov000_0236a444 + 0x40) = (t4 << 0xd);
}
