/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
extern u8 data_02077944[];
s32 func_ov000_0230e5dc();
s32 func_02068df8();

void func_ov000_0230e6e8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)data_022c4560 + 0x20);
    s32 v1 = t0;
    if ((*(u32 *)((u8 *)data_022c4560 + 0x20) & 2) != 0) {
        s32 t2 = *(s8 *)((u8 *)data_02077944 + (v1 & 3));
        *(u8 *)((u8 *)p0 + 0x7) = t2;
        s32 t3 = *(u32 *)((u8 *)p0 + 0x204);
        if (t3 != 0) {
            *(u8 *)((u8 *)t3 + 0x7) = *(s8 *)((u8 *)p0 + 0x7);
        }
        v1 = t3;
    }
    if ((*(u32 *)((u8 *)p0 + 0x58) & 4) != 0) {
        func_ov000_0230e5dc(p0, v1);
        return;
    }
    s32 t4 = func_02068df8(*(s16 *)((u8 *)p0 + 0x34), 0x18, 0xf00);
    *(u16 *)((u8 *)p0 + 0x34) = t4;
    s32 t5 = (*(u32 *)((u8 *)p0 + 0x58) & 1);
    if (t5 != 0) {
        *(u16 *)((u8 *)p0 + 0x30) = 0;
    }
}
