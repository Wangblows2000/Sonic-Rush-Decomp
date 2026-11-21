#include <nitro/types.h>

void FX_Atan2(s32 a, s32 b);

void Boss1Neck_CalcTurretAngle(s32 x, s32 unused, s32 y, s32 flag) {
    s32 px = x << 4;
    s32 py = y << 4;
    s32 ox, oy;
    if (flag != 0) {
        ox = -0xaa8da;
        oy = 0x2a1290;
        FX_Atan2(px + ox, py + oy);
    } else {
        ox = -0x117726;
        oy = 0x2a1290;
        FX_Atan2(px + ox, py + oy);
    }
}
