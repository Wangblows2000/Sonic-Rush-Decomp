/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gEnemyRoomHudTask[];
extern u8 data_ov000_0236a990[];
s32 ObjData_Release();
s32 StageObj_Destructor();

void EnemyRoomHud_Destructor(s32 p0) {
    *(u32 *)gEnemyRoomHudTask = 0;
    ObjData_Release(data_ov000_0236a990, gEnemyRoomHudTask, 0);
    ObjData_Release(data_ov000_0236a990);
    StageObj_Destructor(p0);
}
