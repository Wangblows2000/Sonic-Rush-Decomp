/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 BossFPartD8_CheckKnockOffRider();
extern u8 gBossF[];
extern u8 gPlayers[];
s32 BossHelp_Destroy();
void BossFPartD8_State_HandRideStartWait();

void BossFPartD8_State_HandRideReturn(u8 *p0) {
    BossFPartD8_CheckKnockOffRider();
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gBossF + 0x21c);
    *(u32 *)((u8 *)*(u32 *)gPlayers + 0x70) = t0;
    s32 t1 = (*(u16 *)((u8 *)*(u32 *)gPlayers + 0x5c) | 0x40);
    s32 t2 = *(u32 *)gPlayers;
    *(u16 *)((u8 *)t2 + 0x5c) = t1;
    *(u32 *)((u8 *)p0 + 0x1b8) = (u32)p0;
    BossHelp_Destroy(t1, t2, t0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)BossFPartD8_State_HandRideStartWait;
}
