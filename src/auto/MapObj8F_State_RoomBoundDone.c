/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Gimmick_GetRoomIndex();
s32 EnemyRoom_Get();
s32 Gimmick_CheckRoomResetOnDeath();

void MapObj8F_State_RoomBoundDone(u8 *p0) {
    s32 t0 = Gimmick_GetRoomIndex();
    s32 t1 = EnemyRoom_Get(1, t0);
    if (t1 == 0) {
        s32 t2 = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        *(u32 *)((u8 *)p0 + 0x54) = t2;
        return;
    }
    s32 t3 = Gimmick_CheckRoomResetOnDeath(t1, 0);
    if (t3 != 0) {
        *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) & ~0x10);
        *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
        return;
    }
    if ((*(u32 *)((u8 *)t1 + 0x20c) & 8) != 0) {
        return;
    }
    *(u32 *)((u8 *)p0 + 0x1f4) = (*(u32 *)((u8 *)p0 + 0x1f4) | 0x10);
    *(u32 *)((u8 *)p0 + 0x54) = (*(u32 *)((u8 *)p0 + 0x54) | 4);
}
