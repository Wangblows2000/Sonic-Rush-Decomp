/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02307e84();
s32 func_ov000_0234237c();
s32 func_ov000_02307dfc();

void GmkWall_State_Unk_ov000_02308a20(u8 *p0) {
    s32 t0 = func_ov000_02307e84();
    s32 t1 = func_ov000_0234237c(1, t0);
    s32 t2 = func_ov000_02307dfc(t1, 0);
    if (t2 == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
}
