/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();
s32 EnemyFlapperGroup_FireShot();
extern u8 gSaveData[];
extern u8 gGameState[];
s32 EnemyFlapperGroup_Action_Hover();
s32 EnemyFlapperGroup_Action_Patrol();

void EnemyFlapperGroup_State_FireShot_ov000_02300338(u8 *p0) {
    if (((s32)*(s16 *)((u8 *)p0 + 0xa2) >> 4) == 0xc) {
        if ((*(u32 *)((u8 *)p0 + 0x1f4) & 1) == 0) {
            Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x78);
            Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
            EnemyFlapperGroup_FireShot(p0, 0, 0);
        }
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 1);
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0x8) = 0x78;
        if ((*(u32 *)((u8 *)gSaveData + 0x8) & 1) != 0) {
            s32 t1 = *(u32 *)((u8 *)gGameState + 0x10);
            if (t1 != 2) {
                if (t1 != 3) {
                    *(u32 *)((u8 *)p0 + 0x8) = (*(u32 *)((u8 *)p0 + 0x8) + ((u32)*(u32 *)((u8 *)p0 + 0x8) >> 1));
                }
            }
        }
        s32 t2 = *(u32 *)((u8 *)p0 + 0x1e0);
        if (*(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x6) == 0) {
            if (*(u8 *)((u8 *)t2 + 0x8) == 0) {
                EnemyFlapperGroup_Action_Hover(p0, t2);
                return;
            }
        }
        EnemyFlapperGroup_Action_Patrol(p0);
        return;
    }
    *(u32 *)((u8 *)p0 + 0x8) = (t0 - 0x1);
}
