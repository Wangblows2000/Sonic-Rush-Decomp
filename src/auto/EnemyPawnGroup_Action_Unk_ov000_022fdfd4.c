/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Enemy_Unk_ov000_022faf48();
void EnemyPawnGroup_State_Unk_ov000_022fdf9c();

void EnemyPawnGroup_Action_Unk_ov000_022fdfd4(u8 *p0) {
    Enemy_Unk_ov000_022faf48(p0, 3);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_Unk_ov000_022fdf9c;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
}
