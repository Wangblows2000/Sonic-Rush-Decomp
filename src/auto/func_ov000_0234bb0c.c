/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_023470ec();
s32 func_ov000_0234c644();
s32 func_ov000_0234ab34();
s32 func_ov000_0234729c();
s32 func_ov000_023470a0();
extern u8 data_022c4ef0[];
extern u8 data_022c4560[];
s32 func_ov000_02332db8();

void func_ov000_0234bb0c(u8 *p0) {
    s32 t0 = func_ov000_023470ec();
    if (t0 != 0) {
        func_ov000_0234c644(p0);
        func_ov000_0234ab34(p0);
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x422) & 3) != 0) {
        if (*(u32 *)((u8 *)p0 + 0x340) != 0) {
            s32 t1 = *(s16 *)((u8 *)p0 + 0x352);
            s32 v2 = t1;
            if (t1 >= 0x14) {
                *(u16 *)((u8 *)p0 + 0x352) = 0x14;
                v2 = 0x14;
            }
            func_ov000_0234c644(p0, v2);
            ((s32 (*)())*(u32 *)((u8 *)p0 + 0x340))(p0, *(u32 *)((u8 *)p0 + 0x340));
            return;
        }
    }
    func_ov000_0234729c(p0);
    s32 t3 = *(s16 *)((u8 *)p0 + 0x34);
    if (t3 < 0x80) {
        if (t3 > -128) {
            *(u16 *)((u8 *)p0 + 0x34) = 0;
            func_ov000_0234c644(p0, 0);
            ((s32 (*)())*(u32 *)((u8 *)p0 + 0x334))(p0, *(u32 *)((u8 *)p0 + 0x334));
            return;
        }
    }
    func_ov000_023470a0(p0);
    if ((*(u32 *)data_022c4ef0 & 0x100) != 0) {
        return;
    }
    if (*(u8 *)((u8 *)p0 + 0x324) != 1) {
        return;
    }
    if ((*(u32 *)((u8 *)data_022c4560 + 0x18) & 1) == 0) {
        return;
    }
    if ((*(u32 *)((u8 *)data_022c4560 + 0x20) & 7) != 0) {
        return;
    }
    func_ov000_02332db8(p0, (*(u32 *)((u8 *)data_022c4560 + 0x18) & 1));
}
