/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Gimmick_GetRoomIndex();
s32 EnemyRoom_Get();
s32 Gimmick_CheckRoomResetOnDeath();
s32 MapObj8D_Action_Done();

void MapObj8D_State_WaitEnemyDefeated(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x8);
    s32 t1 = Gimmick_GetRoomIndex();
    s32 t2 = EnemyRoom_Get(1, t1);
    if (t2 == 0) {
        s32 t3 = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)p0 + 0x54) = t3;
        return;
    }
    s32 t4 = Gimmick_CheckRoomResetOnDeath(t2, 0);
    if (t4 != 0) {
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~0x10);
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)t0 + 0x54) = (*(u32 *)((u8 *)t0 + 0x54) | 4);
        return;
    }
    if ((*(u32 *)((u8 *)t0 + 0x54) & 4) == 0) {
        return;
    }
    *(u8 *)((u8 *)t2 + 0x213) = (*(s8 *)((u8 *)(t2 + 0x200) + 0x13) + 0x1);
    MapObj8D_Action_Done(p0);
}
