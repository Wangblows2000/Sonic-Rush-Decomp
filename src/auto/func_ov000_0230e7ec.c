/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c4560[];
extern u8 data_02077944[];
s32 func_ov000_0230e770();

void func_ov000_0230e7ec(u8 *p0) {
    s32 t0 = *(s8 *)((u8 *)data_02077944 + (*(u32 *)((u8 *)data_022c4560 + 0x20) & 3));
    *(u8 *)((u8 *)p0 + 0x7) = t0;
    s32 t1 = *(u32 *)((u8 *)p0 + 0x204);
    if (t1 != 0) {
        *(u8 *)((u8 *)t1 + 0x7) = *(s8 *)((u8 *)p0 + 0x7);
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
    s32 t2 = *(u32 *)((u8 *)p0 + 0xc);
    if (t2 == 0) {
        func_ov000_0230e770(p0, t2, t1);
        return;
    }
    s32 t3 = (*(u32 *)((u8 *)p0 + 0x58) & 1);
    if (t3 != 0) {
        *(u16 *)((u8 *)p0 + 0x30) = 0;
    }
}
