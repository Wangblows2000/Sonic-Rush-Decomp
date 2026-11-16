/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4680[];
extern u8 data_022c4560[];
s32 func_020446b0();
s32 func_0204b7ec();
extern u8 data_ov000_0236ac60[];
s32 func_02068204();
s32 func_ov000_022faf48();
s32 func_ov000_022fe7e8();

void func_ov000_022fd410(u8 *p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1f4);
    if ((*(u32 *)((u8 *)p0 + 0x1f4) & 1) == 0) {
        if (*(u32 *)((u8 *)p0 + 0xc) == 0) {
            return;
        }
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
        if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
            return;
        }
        *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 1);
        *(u32 *)((u8 *)p0 + 0xc) = 0x6d;
        if ((*(u32 *)((u8 *)data_022c4680 + 0x8) & 1) == 0) {
            return;
        }
        if (*(u32 *)((u8 *)data_022c4560 + 0x10) == 2) {
            return;
        }
        s32 t1 = *(u32 *)((u8 *)data_022c4560 + 0x10);
        if (t1 != 3) {
            *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + ((s32)*(u32 *)((u8 *)p0 + 0xc) >> 2));
        }
        return;
    }
    if ((t0 & 4) == 0) {
        if (((s32)*(s16 *)((u8 *)p0 + 0xa2) >> 4) == 0x2c) {
            func_020446b0(8, (t0 & 4), t0, p3);
            func_0204b7ec(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x75);
            func_02068204(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)data_ov000_0236ac60);
        }
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
        if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
            return;
        }
        func_ov000_022faf48(p0, 3);
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 4);
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x1f4) = (t0 & ~5);
    func_ov000_022fe7e8(p0, (t0 & ~5));
}
