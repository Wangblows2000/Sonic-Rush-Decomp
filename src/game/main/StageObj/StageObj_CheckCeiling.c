#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[3];
    u8 unk5;
    u8 pad6[0x10 - 6];
    s32 x;
    s32 y;
    u8 pad18[0x58 - 0x18];
    u32 flags;
    u8 pad5C[0x64 - 0x5c];
    s8 unk64;
    s8 unk65;
    s8 unk66;
    s8 unk67;
} UnkPlayer_02065624;

u8 StageObj_GetMapProbeFlags(UnkPlayer_02065624 *p);
s32 StageObj_ProbeCollision(UnkPlayer_02065624 *p, s32 x, s32 y, u8 layer, u8 dir, u8 *angle, u8 *attr);

BOOL StageObj_CheckCeiling(UnkPlayer_02065624 *p) {
    s32 d2;
    u8 dir;
    s32 dx2;
    s32 dx1;
    s32 dy1;
    s32 dy2;
    s32 x;
    s32 d;
    u8 q;
    s32 y;
    u8 layer;

    layer = StageObj_GetMapProbeFlags(p);
    x = p->x >> 8;
    y = p->y >> 8;
    if (!(p->flags & 0x10))
        q = ((p->unk1 + 0x20) & 0xc0) >> 6;
    else
        q = 0;
    if (p->unk5 != 0)
        q = (u8)(q + (u8)(((p->unk5 + 0x20) & 0xc0) >> 6)) & 3;
    switch (q) {
    case 0:
    default:
        dy1 = p->unk65;
        dx1 = (s8)(p->unk66 - 2);
        dir = 3;
        dx2 = (s8)(p->unk64 + 2);
        dy2 = dy1;
        break;
    case 1:
        dx1 = p->unk66;
        dir = 0;
        dy1 = (s8)(p->unk64 + 2);
        dy2 = (s8)(dx1 - 2);
        dx2 = dx1;
        break;
    case 2:
        dy1 = p->unk67;
        dx1 = (s8)(p->unk66 - 2);
        dir = 2;
        dx2 = (s8)(p->unk64 + 2);
        dy2 = dy1;
        break;
    case 3:
        dx1 = p->unk64;
        dir = 1;
        dy1 = (s8)(dx1 + 2);
        dx2 = dx1;
        dy2 = (s8)(p->unk66 - 2);
        break;
    }
    d = (s8)StageObj_ProbeCollision(p, x + dx1, y + dy1, layer, dir, NULL, NULL);
    d2 = (s8)StageObj_ProbeCollision(p, x + dx2, y + dy2, layer, dir, NULL, NULL);
    if (d >= d2)
        d = d2;
    return d <= 0;
}
