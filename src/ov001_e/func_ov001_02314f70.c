#include <nitro/types.h>

typedef struct UnkStruct_02314f70_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314f70_v;

typedef struct UnkStruct_02314f70 {
    u8 pad0[0x20];
    UnkStruct_02314f70_v pos;
    UnkStruct_02314f70_v target;
    u8 pad38[0x44 - 0x38];
    u16 lat;
    u16 lon;
} UnkStruct_02314f70;

u16 FX_Atan2(s32 y, s32 x);

static inline u16 Atan(s32 y, s32 x) {
    if (x == 0)
        return 0x4000;
    return FX_Atan2(y, x);
}

void func_ov001_02314f70(UnkStruct_02314f70 *e) {
    s32 dx = e->pos.x - e->target.x;
    s32 dy = e->pos.y - e->target.y;
    s32 dz = e->pos.z - e->target.z;

    if (dy == 0)
        e->lat = 0x4000;
    else if (dz > 0)
        e->lat = Atan(dz, dy);
    else
        e->lat = Atan(-dz, dy);
    if (dz == 0) {
        if (dx > 0)
            e->lon = 0x4000;
        else
            e->lon = 0xc000;
    } else {
        e->lon = FX_Atan2(dx, dz);
    }
}
