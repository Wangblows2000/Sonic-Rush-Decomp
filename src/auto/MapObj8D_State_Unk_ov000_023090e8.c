/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02307e84();
s32 func_ov000_0234237c();
s32 func_ov000_02307dfc();
s32 MapObj8D_Action_Unk_ov000_023090d8();

void MapObj8D_State_Unk_ov000_023090e8(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    s32 t1 = func_ov000_02307e84();
    s32 t2 = func_ov000_0234237c(1, t1);
    if (t2 == 0) {
        s32 t3 = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)p0 + 0x54) = t3;
        return;
    }
    s32 t4 = func_ov000_02307dfc(t2, 0);
    if (t4 != 0) {
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~0x10);
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) | 4);
        return;
    }
    if ((*(u32 *)((u8 *)t0 + 0x54) & 4) == 0) {
        return;
    }
    *(u8 *)((u8 *)t2 + 0x213) = (*(s8 *)((u8 *)(t2 + 0x200) + 0x13) + 0x1);
    MapObj8D_Action_Unk_ov000_023090d8(p0);
}
