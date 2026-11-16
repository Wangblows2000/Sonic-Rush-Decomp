/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void FixVs_State_FollowPlayer(u8 *p0) {
    if (*(u32 *)((u8 *)p0 + 0x88) == 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x88) + 0x10);
    *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) - 0x2000);
    *(u32 *)((u8 *)p0 + 0x18) = *(u32 *)((u8 *)t0 + 0x18);
    *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x1);
    s32 t1 = *(u32 *)((u8 *)p0 + 0xc);
    if (t1 > 0xf0) {
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
    }
}
