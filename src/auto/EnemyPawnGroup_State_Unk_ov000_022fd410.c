/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gSaveData[];
extern u8 gGameState[];
s32 ScreenShake_Start();
s32 Sound_PlaySeqArcEx();
extern u8 gPlayers[];
s32 Sound_SetPositional();
s32 Enemy_DisableCollidersSetAnim();
s32 EnemyPawnGroup_Action_Walk();

void EnemyPawnGroup_State_Unk_ov000_022fd410(u8 *p0, s32 p1, s32 p2, s32 p3) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1f4);
    if ((*(u32 *)((u8 *)p0 + 0x1f4) & 1) == 0) {
        if (*(u32 *)((u8 *)p0 + 0xc) == 0) {
            return;
        }
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
        if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
            return;
        }
        *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 1);
        *(u32 *)((u8 *)p0 + 0xc) = 0x6d;
        if ((*(u32 *)((u8 *)gSaveData + 0x8) & 1) == 0) {
            return;
        }
        if (*(u32 *)((u8 *)gGameState + 0x10) == 2) {
            return;
        }
        s32 t1 = *(u32 *)((u8 *)gGameState + 0x10);
        if (t1 != 3) {
            *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + ((s32)*(u32 *)((u8 *)p0 + 0xc) >> 2));
        }
        return;
    }
    if ((t0 & 4) == 0) {
        if (((s32)*(s16 *)((u8 *)p0 + 0xa2) >> 4) == 0x2c) {
            ScreenShake_Start(8, (t0 & 4), t0, p3);
            Sound_PlaySeqArcEx(*(u32 *)((u8 *)p0 + 0x1dc), 0, 0x75);
            Sound_SetPositional(*(u32 *)((u8 *)p0 + 0x1dc), p0, *(u32 *)gPlayers);
        }
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) - 0x1);
        if (*(u32 *)((u8 *)p0 + 0xc) != 0) {
            return;
        }
        Enemy_DisableCollidersSetAnim(p0, 3);
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 4);
        return;
    }
    if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x1f4) = (t0 & ~5);
    EnemyPawnGroup_Action_Walk(p0, (t0 & ~5));
}
