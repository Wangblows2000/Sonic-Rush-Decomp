/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBoss3[];
s32 Boss3_Action_WaitThrowAttack();
s32 Boss3PartCC_Action_StartThrowAttack();

void Boss3_StartThrowAttack(s32 p0) {
    s32 t0 = (*(u32 *)gBoss3 + 0x200);
    *(u16 *)((u8 *)t0 + 0x22) = p0;
    Boss3_Action_WaitThrowAttack(*(u32 *)gBoss3, t0, gBoss3);
    Boss3PartCC_Action_StartThrowAttack(*(u32 *)((u8 *)*(u32 *)gBoss3 + 0x214));
}
