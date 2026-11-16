/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 EnemyRoom_Get();
s32 EnemyRoom_ClearActive();
s32 MapObj8C_Action_Idle();

void MapObj8C_State_FinishRoom(u8 *p0) {
    s32 t0 = EnemyRoom_Get(1, *(u8 *)((u8 *)p0 + 0x214));
    if (t0 == 0) {
        return;
    }
    EnemyRoom_ClearActive(*(u8 *)((u8 *)p0 + 0x214));
    MapObj8C_Action_Idle(p0);
}
