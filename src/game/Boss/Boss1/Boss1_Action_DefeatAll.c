/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 Boss1_Action_Defeat();
s32 Boss1Neck_Action_Defeat();
s32 Boss1Hammer_Action_Defeat();

void Boss1_Action_DefeatAll(void) {
    Boss1_Action_Defeat(*(u32 *)gBoss1);
    Boss1Neck_Action_Defeat(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x210));
    Boss1Hammer_Action_Defeat(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214));
}
