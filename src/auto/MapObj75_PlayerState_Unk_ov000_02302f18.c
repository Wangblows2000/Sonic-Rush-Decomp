/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapObj75_Unk_ov000_02302e2c();
s32 MapObj75_PlayerState_Unk_ov000_02303310();

void MapObj75_PlayerState_Unk_ov000_02302f18(s32 p0, s32 p1) {
    s32 t0 = MapObj75_Unk_ov000_02302e2c();
    if (t0 != 0) {
        return;
    }
    MapObj75_PlayerState_Unk_ov000_02303310(p0, p1);
}
