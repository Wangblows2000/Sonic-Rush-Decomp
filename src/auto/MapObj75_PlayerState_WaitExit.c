/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapObj75_IsPlayerInArea();
s32 MapObj75_PlayerState_ReleaseBounds();

void MapObj75_PlayerState_WaitExit(s32 p0, s32 p1) {
    s32 t0 = MapObj75_IsPlayerInArea();
    if (t0 != 0) {
        return;
    }
    MapObj75_PlayerState_ReleaseBounds(p0, p1);
}
