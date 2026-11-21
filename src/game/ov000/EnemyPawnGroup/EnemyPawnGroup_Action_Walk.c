/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Enemy_DisableCollidersSetAnim();
void EnemyPawnGroup_State_Walk();

void EnemyPawnGroup_Action_Walk(u8 *p0) {
    Enemy_DisableCollidersSetAnim(p0, 0);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x100);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_Walk;
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x1f4) & 1);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0xc) = 0x10;
    }
}
