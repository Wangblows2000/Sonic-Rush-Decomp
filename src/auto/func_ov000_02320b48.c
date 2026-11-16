/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_02068db4();
extern u8 data_ov000_0236aad8[];
s32 func_ov000_02320f40();

void func_ov000_02320b48(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 != 0) {
        if (*(u32 *)((u8 *)t0 + 0x190) == 0) {
            s32 t1 = func_02068db4(*(s16 *)((u8 *)p0 + 0x2e), 0x20);
            *(u16 *)((u8 *)p0 + 0x2e) = t1;
            s32 t2 = func_02068db4(*(s16 *)((u8 *)p0 + 0x30), 0x20);
            *(u16 *)((u8 *)p0 + 0x30) = t2;
        }
    }
    s32 t3 = ((*(u16 *)((u8 *)data_ov000_0236aad8 + 0x62) + 0x8) << 8);
    s32 t4 = *(u32 *)((u8 *)p0 + 0x14);
    if (t4 < t3) {
        *(u32 *)((u8 *)p0 + 0x14) = t3;
    }
    s32 t5 = *(s16 *)((u8 *)p0 + 0x2e);
    s32 v6 = t5;
    if (t5 < 0) {
        v6 = -v6;
    }
    if (v6 >= 0x40) {
        return;
    }
    s32 t7 = *(s16 *)((u8 *)p0 + 0x30);
    s32 v8 = t7;
    if (t7 < 0) {
        v8 = -v8;
    }
    if (v8 >= 0x40) {
        return;
    }
    func_ov000_02320f40(p0, t4);
}
