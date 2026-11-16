/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_0230023c();

void func_ov000_02300844(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    s32 v1;
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x8) = (t0 - 0x1);
        s32 t2 = *(u32 *)((u8 *)p0 + 0x8);
        s32 v3 = v1;
        if (t2 == 0) {
            s32 t4 = (*(u16 *)((u8 *)p0 + 0x196) | 4);
            *(u16 *)((u8 *)p0 + 0x196) = t4;
            v3 = t4;
        }
        v1 = v3;
    }
    s32 t5 = *(u16 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x2);
    if (t5 != 0x10) {
        return;
    }
    func_ov000_0230023c(p0, t5, v1);
}
