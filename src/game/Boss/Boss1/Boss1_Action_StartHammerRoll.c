/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss1[];
s32 Boss1_Action_WaitHammerRoll();
s32 Boss1Neck_Action_StartHammerRoll();
s32 Boss1Hammer_Action_StartRoll();

void Boss1_Action_StartHammerRoll(s32 p0) {
    s32 t0 = (*(u32 *)gBoss1 + 0x200);
    *(u16 *)((u8 *)t0 + 0x28) = p0;
    Boss1_Action_WaitHammerRoll(*(u32 *)gBoss1, t0, gBoss1);
    Boss1Neck_Action_StartHammerRoll(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x210));
    Boss1Hammer_Action_StartRoll(*(u32 *)((u8 *)*(u32 *)gBoss1 + 0x214));
}
