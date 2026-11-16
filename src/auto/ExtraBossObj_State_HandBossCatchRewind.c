/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ExtraBoss_HandBoss_SelectNextAttack();

void ExtraBossObj_State_HandBossCatchRewind(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x84);
    s32 t1 = *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1dc) + 0x18) + 0x84);
    *(u32 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0xe4) = (*(u32 *)*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x84) + 0xe4) - 0x1000);
    s32 t2 = *(u32 *)((u8 *)t1 + 0xe4);
    *(u32 *)t2 = (*(u32 *)*(u32 *)((u8 *)t1 + 0xe4) - 0x1000);
    s32 t3 = *(u32 *)*(u32 *)((u8 *)t0 + 0xe4);
    if (t3 > 0) {
        return;
    }
    ExtraBoss_HandBoss_SelectNextAttack(p0, t3, t2, t1);
}
