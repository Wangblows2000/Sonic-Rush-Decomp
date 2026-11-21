/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 Boss1_Action_Idle();
s32 Boss1Neck_Action_FollowPlayer();
s32 Boss1Hammer_Action_Idle_ov000_022c696c();

void Boss1_Action_IdleAll(void) {
    Boss1_Action_Idle(*(u32 *)gBoss1);
    Boss1Neck_Action_FollowPlayer(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x210));
    Boss1Hammer_Action_Idle_ov000_022c696c(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214));
}
