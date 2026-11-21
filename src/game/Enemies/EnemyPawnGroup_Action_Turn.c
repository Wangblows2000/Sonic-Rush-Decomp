/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Enemy_DisableCollidersSetAnim();
void EnemyPawnGroup_State_Turn();

void EnemyPawnGroup_Action_Turn(u8 *p0) {
    Enemy_DisableCollidersSetAnim(p0, 1);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_Turn;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x1f4) & 1);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x18);
    }
}
