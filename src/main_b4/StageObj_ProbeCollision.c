#include <nitro/types.h>

typedef struct {
    u8 pad0[0x58];
    u32 flags;
} UnkPlayer_02066010;

s32 MapCollision_ProbeDistanceLong(s32 x, s32 y, u8 layer, u8 dir, u8 *angle, u8 *attr);
s32 ObjCollisionMap_ProbeForObj(UnkPlayer_02066010 *p, s32 x, s32 y, u8 layer, u8 dir, u8 *angle, u8 *attr);

s32 StageObj_ProbeCollision(UnkPlayer_02066010 *p, s32 x, s32 y, u8 layer, u8 dir, u8 *angle, u8 *attr) {
    s32 d = 0x20;
    s32 d2;
    if (!(p->flags & 0x1000))
        d = MapCollision_ProbeDistanceLong(x, y, layer, dir, angle, attr);
    if (!(p->flags & 0x200)) {
        d2 = ObjCollisionMap_ProbeForObj(p, x, y, layer, dir, angle, attr);
        if (d > d2)
            d = d2;
    }
    return d;
}
