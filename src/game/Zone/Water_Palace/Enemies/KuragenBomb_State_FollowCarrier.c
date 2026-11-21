/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 KuragenBomb_Action_Drop();

void KuragenBomb_State_FollowCarrier(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x88);
    if (t0 == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x10) = *(u32 *)((u8 *)t0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) + 0xf00);
    s32 t1 = (*(u32 *)((u8 *)t0 + 0x238) & 0x20);
    if (t1 == 0) {
        return;
    }
    KuragenBomb_Action_Drop(p0, t1);
    *(u32 *)((u8 *)t0 + 0x238) = (*(u32 *)((u8 *)t0 + 0x238) & ~0x20);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 0x200);
}
