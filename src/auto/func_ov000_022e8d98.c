/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369b78[];
s32 func_ov000_022e9990();
s32 func_ov000_0232727c();
s32 func_ov000_02326f20();
void func_ov000_022e98d8();

void func_ov000_022e8d98(u8 *p0) {
    s32 t0 = *(u32 *)data_ov000_02369b78;
    func_ov000_022e9990(p0, *(s16 *)((u8 *)p0 + 0x34), data_ov000_02369b78);
    if (*(u32 *)((u8 *)p0 + 0x3d0) != 0) {
        if ((*(u32 *)((u8 *)t0 + 0x220) & 1) == 0) {
            func_ov000_0232727c(p0, 0, 0, 0);
        }
    }
    if (*(s16 *)((u8 *)p0 + 0x2e) != 0) {
        if ((*(u32 *)((u8 *)p0 + 0x58) & 4) == 0) {
            if ((*(u16 *)((u8 *)p0 + 0x3dc) & 0x40) == 0) {
                return;
            }
        }
    }
    s32 t1 = *(u32 *)((u8 *)p0 + 0x480);
    s32 v2 = t1;
    if (t1 != 0) {
        func_ov000_02326f20(v2);
        *(u32 *)((u8 *)p0 + 0x480) = 0;
        v2 = 0;
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0x488);
    if (t3 != 0) {
        *(u32 *)((u8 *)t3 + 0x54) = (*(u32 *)((u8 *)t3 + 0x54) | 8);
        *(u32 *)((u8 *)p0 + 0x488) = 0;
    }
    *(u16 *)((u8 *)p0 + 0x2e) = ((s32)*(s16 *)((u8 *)p0 + 0x2e) >> 1);
    *(u32 *)((u8 *)p0 + 0x440) = (u32)func_ov000_022e98d8;
}
