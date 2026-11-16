/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 EnemyPawnGroup_Action_Turn();
s32 Enemy_DisableCollidersSetAnim();

void EnemyPawnGroup_State_Unk_ov000_022fcaa8(u8 *p0) {
    if ((*(u32 *)((u8 *)p0 + 0x58) & 1) == 0) {
        return;
    }
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1f4);
    if ((*(u32 *)((u8 *)p0 + 0x1f4) & 1) != 0) {
        if ((*(u16 *)((u8 *)p0 + 0x5c) & 8) == 0) {
            return;
        }
        *(u32 *)((u8 *)p0 + 0x1f4) = (t0 & ~1);
        EnemyPawnGroup_Action_Turn(p0, (t0 & ~1), t0);
        return;
    }
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    s32 t1 = (*(u32 *)((u8 *)p0 + 0x1f4) | 1);
    *(u32 *)((u8 *)p0 + 0x1f4) = t1;
    Enemy_DisableCollidersSetAnim(p0, 2, t1);
}
