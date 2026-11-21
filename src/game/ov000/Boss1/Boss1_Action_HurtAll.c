/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 Boss1_Action_Hurt();
s32 Boss1Neck_Action_Hurt();
s32 Boss1Hammer_Action_Hurt();

void Boss1_Action_HurtAll(void) {
    Boss1_Action_Hurt(*(u32 *)gBoss1);
    Boss1Neck_Action_Hurt(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x210));
    Boss1Hammer_Action_Hurt(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214));
}
