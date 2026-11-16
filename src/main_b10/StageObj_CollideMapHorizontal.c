#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[3];
    u8 unk5;
    u8 pad6[0x10 - 6];
    s32 x;
    s32 y;
    u8 pad18[0x28 - 0x18];
    s16 unk28;
    u8 pad2A[0x2e - 0x2a];
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 flags;
    u16 unk5C;
    u8 pad5E[0x64 - 0x5e];
    s8 unk64;
    s8 unk65;
    s8 unk66;
    s8 unk67;
} UnkPlayer_020657dc;

u8 StageObj_GetMapProbeFlags(UnkPlayer_020657dc *p);
BOOL StageObj_CheckCeiling(UnkPlayer_020657dc *p);
s32 StageObj_ProbeCollision(UnkPlayer_020657dc *p, s32 x, s32 y, u8 layer, u8 dir, u8 *angle, u8 *attr);

void StageObj_CollideMapHorizontal(UnkPlayer_020657dc *p, BOOL noMove, s32 spd) {
    u8 ang;
    u8 a1;
    s8 dx1 = 0;
    s32 d;
    s8 dy1 = 0;
    s8 dx2 = 0;
    s8 dy2 = 0;
    u8 layer;
    s32 q;
    u8 dir;
    u8 flip;
    s32 extra;
    s32 x;
    s32 y;
    s32 d2;
    s32 e;
    s32 e2;

    layer = StageObj_GetMapProbeFlags(p);
    flip = 0;
    x = p->x >> 8;
    y = p->y >> 8;
    extra = 0;
    a1 = p->unk1;
    q = ((a1 + 0x20) & 0xc0) >> 6;
    ang = 0x40;
    if (q == 2)
        ang += 0x80;
    if (spd > 0) {
        ang = -ang;
        flip = 1;
    } else if (!(p->unk5C & 1)) {
        ang = -ang;
        flip = 1;
    }
    if (p->unk5 != 0) {
        ang += p->unk5;
        if (spd > 0) {
            ang = -ang;
            flip = 0;
        }
    }
    if (q == 2)
        flip ^= 1;
    if (!(p->flags & 0x10))
        ang += a1;
    if (StageObj_CheckCeiling(p))
        extra = 6;

    switch (((ang + 0x20) & 0xc0) >> 6) {
    case 0:
        dy1 = p->unk66 + 2;
        dx1 = p->unk67 - 4;
        dx2 = p->unk65 + 4;
        dy2 = dy1;
        dir = 2;
        break;
    case 1:
        dx1 = p->unk64 - 2;
        dy1 = p->unk67 - 4;
        dy2 = p->unk65 + extra;
        dx2 = dx1;
        if (flip) {
            dy1 = -dy1;
            dy2 = -dy2;
        }
        dir = 1;
        break;
    case 2:
        dy1 = p->unk64 - 2;
        dx1 = p->unk67 - 4;
        dir = 3;
        dy2 = dy1;
        dx2 = p->unk65 + 4;
        break;
    case 3:
        dir = 0;
        dx1 = p->unk66 + 2;
        dy1 = -(p->unk67 - 4);
        dy2 = -(p->unk65 + extra);
        dx2 = dx1;
        if (flip) {
            dy1 = -dy1;
            dy2 = -dy2;
        }
        break;
    }
    d = (s8)StageObj_ProbeCollision(p, x + dx1, y + dy1, layer | 0x80, dir, NULL, NULL);
    d2 = (s8)StageObj_ProbeCollision(p, x + dx2, y + dy2, layer | 0x80, dir, NULL, NULL);
    if (d >= d2)
        d = d2;
    if (d <= 0) {
        if (!noMove) {
            switch (dir) {
            default:
            case 2:
                y += d;
                break;
            case 1:
                x -= d;
                break;
            case 3:
                y -= d;
                break;
            case 0:
                x += d;
                break;
            }
        } else {
            p->flags |= 4;
            if (!(p->flags & 0x4000)) {
                if (dir == 1 && p->unk28 < 0) {
                    p->unk34 = 0;
                    p->unk2E = 0;
                }
                if (dir == 0 && p->unk28 > 0) {
                    p->unk34 = 0;
                    p->unk2E = 0;
                }
            }
            if ((dir == 2 || dir == 3) && !(p->flags & 0x4000)) {
                p->unk30 = 0;
                p->unk34 = 0;
            }
        }
    }

    switch ((((u8)(ang + 0x80)) + 0x20 & 0xc0) >> 6) {
    case 0:
        dy1 = p->unk66 + 2;
        dx1 = p->unk67 - 4;
        dx2 = p->unk65 + 4;
        dy2 = dy1;
        dir = 2;
        break;
    case 1:
        dir = 1;
        dx1 = p->unk64 - 2;
        dy1 = -(p->unk67 - 4);
        dy2 = -(p->unk65 + extra);
        dx2 = dx1;
        if (flip) {
            dy1 = -dy1;
            dy2 = -dy2;
        }
        break;
    case 2:
        dy1 = p->unk64 - 2;
        dx1 = p->unk67 - 4;
        dir = 3;
        dy2 = dy1;
        dx2 = p->unk65 + 4;
        break;
    case 3:
        dx1 = p->unk66 + 2;
        dy1 = p->unk67 - 4;
        dy2 = p->unk65 + extra;
        dir = 0;
        dx2 = dx1;
        if (flip) {
            dy1 = -dy1;
            dy2 = -dy2;
        }
        break;
    }
    e = (s8)StageObj_ProbeCollision(p, x + dx1, y + dy1, (u32)layer | 0x80, dir, NULL, NULL);
    e2 = (s8)StageObj_ProbeCollision(p, x + dx2, y + dy2, (u32)layer | 0x80, dir, NULL, NULL);
    if (e >= e2)
        e = e2;
    if (e <= 0) {
        if (!noMove) {
            switch (dir) {
            default:
            case 2:
                y += e;
                break;
            case 1:
                x -= e;
                break;
            case 3:
                y -= e;
                break;
            case 0:
                x += e;
                break;
            }
        } else {
            if (e < 0)
                p->flags |= 8;
            if (dir == 1 && spd < 0)
                p->unk34 = 0;
            if (dir == 0 && spd > 0)
                p->unk34 = 0;
            if (!(p->flags & 0x4000)) {
                if (dir == 1 && p->unk2E < 0)
                    p->unk2E = 0;
                if (dir == 0 && p->unk2E > 0)
                    p->unk2E = 0;
            }
        }
    }
    if (!noMove) {
        p->x -= ((p->x >> 8) - x) << 8;
        p->y -= ((p->y >> 8) - y) << 8;
    }
}
