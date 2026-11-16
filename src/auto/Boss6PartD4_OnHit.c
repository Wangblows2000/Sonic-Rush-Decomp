/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 Boss6_Unk_ov000_022dfd80();

void Boss6PartD4_OnHit(u8 *p0, u8 *p1) {
    u32 t0 = *(u32 *)((u8 *)p1 + 0x18);
    u32 t1 = *(u32 *)((u8 *)p0 + 0x18);
    u8 t2 = *(u8 *)t0;
    if (t2 != 1) {
        return;
    }
    u32 t3 = *(u32 *)gPlayers;
    u32 t4 = *(u32 *)((u8 *)t3 + 0x10);
    u32 t5 = *(u32 *)((u8 *)t3 + 0x14);
    u32 t6 = *(u32 *)((u8 *)t3 + 0x18);
    Boss6_Unk_ov000_022dfd80(3, t4, t5, t6);
    u32 t7 = *(u32 *)((u8 *)t1 + 0x368);
    *(u32 *)((u8 *)t1 + 0x368) = (t7 | 1);
}
