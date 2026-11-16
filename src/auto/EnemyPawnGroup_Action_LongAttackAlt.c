/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Enemy_DisableCollidersSetAnim();
void EnemyPawnGroup_State_LongAttackAlt();
s32 ObjRect_SetBox2D();

void EnemyPawnGroup_Action_LongAttackAlt(u8 *p0) {
    Enemy_DisableCollidersSetAnim(p0, 2);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 4);
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_LongAttackAlt;
    *(u16 *)((u8 *)p0 + 0x238) = 2;
    ObjRect_SetBox2D(((u8 *)p0 + 0x20c), -120, -48, 0x50, -16);
}
