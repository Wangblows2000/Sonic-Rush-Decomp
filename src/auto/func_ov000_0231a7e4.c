/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 MapObject_Spawn();

void func_ov000_0231a7e4(s32 p0, s32 p1, s32 p2, s32 p3) {
    MapObject_Spawn(0xf1, p2, p3, 0, 0, 0, 0, 0, 0);
}
