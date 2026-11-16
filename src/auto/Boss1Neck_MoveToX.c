/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Boss1Neck_MoveTowardX();

void Boss1Neck_MoveToX(s32 p0, s32 p1) {
    *(u16 *)((u8 *)p0 + 0x21a) = 0;
    Boss1Neck_MoveTowardX(p0, p1, 0xc00, 0x180, 0x60, 0x500, 0x500);
}
