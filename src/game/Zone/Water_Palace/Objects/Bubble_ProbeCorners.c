#include <game/Stage/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x216 - 0x20C];
    u8 unk216;
} UnkStruct_02302164;

s32 MapCollision_ProbeDistance(s32 x, s32 y, s32 a, s32 b, s32 c, s32 d);

u8 Bubble_ProbeCorners(UnkStruct_02302164 *w) {
    s32 r;
    u8 hit;
    u8 ret;
    s16 y;
    s16 x;

    hit = 0;
    r = w->unk216;
    y = (s16)((w->base.unk14 >> 8) - 3);
    x = (s16)(w->base.unk10 >> 8);
    ret = 0;
    if (MapCollision_ProbeDistance(x - r, y - r, 0, 3, 0, 0) <= 0 || MapCollision_ProbeDistance(x - r, y - r, 1, 3, 0, 0) <= 0)
        hit |= 1;
    if (MapCollision_ProbeDistance(x + r, y - r, 0, 3, 0, 0) <= 0 || MapCollision_ProbeDistance(x + r, y - r, 1, 3, 0, 0) <= 0)
        hit |= 2;
    if (MapCollision_ProbeDistance(x - r, y + r, 0, 3, 0, 0) <= 0 || MapCollision_ProbeDistance(x - r, y + r, 1, 3, 0, 0) <= 0)
        hit |= 4;
    if (MapCollision_ProbeDistance(x + r, y + r, 0, 3, 0, 0) <= 0 || MapCollision_ProbeDistance(x + r, y + r, 1, 3, 0, 0) <= 0)
        hit |= 8;
    if (hit == 5)
        ret |= 1;
    if (hit == 3)
        ret |= 2;
    if ((hit == 0xa) & 0xa)
        ret |= 4;
    if ((hit == 0xc) & 0xc)
        ret |= 8;
    return ret;
}
