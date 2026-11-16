/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Enemy_Unk_ov000_022faf48();
void EnemyPawnGroup_State_Unk_ov000_022fd410();
extern u8 gSaveData[];
extern u8 gGameState[];

void EnemyPawnGroup_Action_Unk_ov000_022fd564(u8 *p0) {
    Enemy_Unk_ov000_022faf48(p0, 2);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_Unk_ov000_022fd410;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0xc) = 0x10;
    *(u32 *)((u8 *)p0 + 0x8) = 0xb4;
    *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~5);
    if ((*(u32 *)((u8 *)gSaveData + 0x8) & 1) == 0) {
        return;
    }
    if (*(u32 *)((u8 *)gGameState + 0x10) == 2) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)gGameState + 0x10);
    if (t0 != 3) {
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + *(u32 *)((u8 *)p0 + 0xc));
    }
}
