/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Enemy_Unk_ov000_022faf48();
void EnemyPawnGroup_State_Unk_ov000_022fe84c();
s32 EffShadow_Create();

void EnemyPawnGroup_Action_Unk_ov000_022fe9b4(u8 *p0) {
    Enemy_Unk_ov000_022faf48(p0, 0);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) & ~1);
    *(u16 *)((u8 *)p0 + 0x5c) = (*(u16 *)((u8 *)p0 + 0x5c) | 0x10);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x80);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x200);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) | 0x1500);
    s32 t0 = *(s8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x7);
    if (t0 < 0x28) {
        *(u32 *)((u8 *)p0 + 0x18) = -(t0 << 8);
    }
    *(u32 *)((u8 *)p0 + 0xc0) = (*(u32 *)((u8 *)p0 + 0xc0) & ~0x60000);
    *(u32 *)((u8 *)p0 + 0x108) = (*(u32 *)((u8 *)p0 + 0x108) & ~0x60000);
    *(u32 *)((u8 *)p0 + 0xc0) = (*(u32 *)((u8 *)p0 + 0xc0) | 0x20000);
    *(u32 *)((u8 *)p0 + 0x108) = (*(u32 *)((u8 *)p0 + 0x108) | 0x20000);
    *(u32 *)((u8 *)p0 + 0xc) = *(u8 *)((u8 *)*(u32 *)((u8 *)p0 + 0x1e0) + 0x9);
    *(u32 *)((u8 *)p0 + 0x8) = 0xc;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_Unk_ov000_022fe84c;
    EffShadow_Create(p0, 0, (u32)EnemyPawnGroup_State_Unk_ov000_022fe84c, 0xc);
}
