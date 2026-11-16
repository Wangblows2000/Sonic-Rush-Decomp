/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
s32 Player_Action_GroundIdle();
extern u8 gStageMapCollision[];
s32 BossFPartD8_Action_ReturnHome();

void BossFPartD8_State_HurtRecover(s32 p0) {
    *(u16 *)((u8 *)p0 + 0x434) = (*(u16 *)((u8 *)p0 + 0x434) + 0x1);
    if ((u32)*(u16 *)((u8 *)p0 + 0x434) <= (u32)0x3c) {
        return;
    }
    s32 t0 = *(u32 *)gPlayers;
    if ((*(u32 *)((u8 *)*(u32 *)gPlayers + 0x32c) & 0x400) == 0) {
        s32 t1 = (*(u32 *)((u8 *)t0 + 0x58) | 0x80);
        *(u32 *)((u8 *)t0 + 0x58) = t1;
        Player_Action_GroundIdle(*(u32 *)gPlayers, t1, t0);
    }
    *(u32 *)((u8 *)gStageMapCollision + 0x24) = 0x340;
    BossFPartD8_Action_ReturnHome(p0, gStageMapCollision, 0x340);
}
