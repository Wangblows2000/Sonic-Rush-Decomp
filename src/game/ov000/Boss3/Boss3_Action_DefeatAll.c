/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss3[];
s32 Boss3_Action_Defeat();
s32 Boss3PartCC_Action_Defeat();
s32 Boss3PartCD_Action_Defeat();

void Boss3_Action_DefeatAll(void) {
    Boss3_Action_Defeat(*(u32 *)gBoss3);
    Boss3PartCC_Action_Defeat(*(u32 *)((u8 *)*(u32 *)gBoss3 + 0x214));
    Boss3PartCD_Action_Defeat(*(u32 *)((u8 *)*(u32 *)gBoss3 + 0x218));
}
