/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022faf48();
void EnemyPawnGroup_State_Unk_ov000_022fcce4();

void func_ov000_022fcd10(u8 *p0) {
    func_ov000_022faf48(p0, 4);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_Unk_ov000_022fcce4;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    *(u16 *)((u8 *)p0 + 0x62) = 0x80;
}
