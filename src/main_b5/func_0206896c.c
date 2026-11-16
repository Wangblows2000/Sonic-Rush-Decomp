#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 angle;
    u8 pad2[0x32];
    s16 spd;
} UnkObj_0206896c;

s16 func_02068db4(s32 spd, s32 decel);
s16 func_02068df8(s32 spd, s32 accel, s32 max);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 func_0206896c(UnkObj_0206896c *o, s32 v, s32 accel, s32 decel, s16 max) {
    if (ABS(v) < 0x100 && ABS(o->spd) < 0x80) {
        v = 0;
        o->spd = v;
    } else {
        if (v < 0) {
            if (o->spd > 0)
                o->spd = func_02068db4(o->spd, decel);
            o->spd = func_02068df8(o->spd, -accel, max);
        } else {
            if (o->spd < 0)
                o->spd = func_02068db4(o->spd, decel);
            o->spd = func_02068df8(o->spd, accel, max);
        }
        v -= o->spd;
        o->angle = (v & 0xffff) >> 8;
    }
    return v;
}
