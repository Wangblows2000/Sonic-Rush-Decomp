/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022faf48();
void EnemyPawnGroup_State_Unk_ov000_022fcc1c();
extern u8 gSaveData[];
extern u8 gGameState[];

void EnemyPawnGroup_Action_Unk_ov000_022fcc50(u8 *p0) {
    func_ov000_022faf48(p0, 2);
    *(u32 *)((u8 *)p0 + 0x58) = (*(u32 *)((u8 *)p0 + 0x58) & ~0x10);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)EnemyPawnGroup_State_Unk_ov000_022fcc1c;
    *(u16 *)((u8 *)p0 + 0x34) = 0;
    s32 t0 = *(u32 *)((u8 *)p0 + 0x204);
    if (t0 != 0) {
        *(u32 *)((u8 *)p0 + 0x8) = *(u32 *)((u8 *)t0 + 0x10);
        *(u32 *)((u8 *)p0 + 0x204) = 0;
    }
    *(u32 *)((u8 *)p0 + 0xc) = 0x20;
    if ((*(u32 *)((u8 *)gSaveData + 0x8) & 1) == 0) {
        return;
    }
    if (*(u32 *)((u8 *)gGameState + 0x10) == 2) {
        return;
    }
    s32 t1 = *(u32 *)((u8 *)gGameState + 0x10);
    if (t1 != 3) {
        *(u32 *)((u8 *)p0 + 0xc) = (*(u32 *)((u8 *)p0 + 0xc) + ((s32)*(u32 *)((u8 *)p0 + 0xc) >> 1));
    }
}
