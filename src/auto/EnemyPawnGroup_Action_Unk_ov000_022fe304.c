/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Enemy_Unk_ov000_022faf48();
void EnemyPawnGroup_State_Unk_ov000_022fe29c();

void EnemyPawnGroup_Action_Unk_ov000_022fe304(u8 *p0) {
    Enemy_Unk_ov000_022faf48(p0, 1);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_Unk_ov000_022fe29c;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    s32 t0 = (*(u32 *)((u8 *)p0 + 0x1f4) & 1);
    if (t0 == 0) {
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + 0x18);
    }
}
