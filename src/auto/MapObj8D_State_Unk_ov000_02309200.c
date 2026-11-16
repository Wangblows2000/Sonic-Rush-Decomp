/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_02307e84();
s32 func_ov000_0234237c();
s32 func_ov000_02307dfc();
s32 MapObj8D_Action_Unk_ov000_02309188();

void MapObj8D_State_Unk_ov000_02309200(u8 *p0) {
    s32 t0 = func_ov000_02307e84();
    s32 t1 = func_ov000_0234237c(1, t0);
    s32 t2 = func_ov000_02307dfc(t1, 0);
    if (t2 != 0) {
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~0x10);
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        return;
    }
    if (t1 == 0) {
        return;
    }
    if ((*(u32 *)((u8 *)t1 + 0x20c) & 0x100) == 0) {
        return;
    }
    s32 t3 = *(u32 *)((u8 *)p0 + 0x1e0);
    s32 t4 = *(s8 *)((u8 *)(t1 + 0x200) + 0x13);
    if (t4 < *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x9)) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
    if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
        return;
    }
    MapObj8D_Action_Unk_ov000_02309188(p0, t3, t4);
}
