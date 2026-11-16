/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Gimmick_GetRoomIndex();
s32 EnemyRoom_Get();
void MapObj8F_State_Unk_ov000_023086a8();

void MapObj8F_Action_Unk_ov000_02308720(u8 *p0) {
    s32 t0 = Gimmick_GetRoomIndex();
    EnemyRoom_Get(1, t0);
    s32 t1 = *(u32 *)((u8 *)p0 + 0x14);
    *(u32 *)((u8 *)p0 + 0x20c) = *(u32 *)((u8 *)p0 + 0x10);
    *(u32 *)((u8 *)p0 + 0x210) = t1;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)MapObj8F_State_Unk_ov000_023086a8;
}
