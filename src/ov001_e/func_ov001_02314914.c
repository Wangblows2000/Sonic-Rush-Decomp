#include <nitro/types.h>

typedef struct UnkStruct_02314914_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_02314914_v;

typedef struct UnkStruct_02314914_e {
    u8 pad0[0x20];
    UnkStruct_02314914_v pos;
    UnkStruct_02314914_v target;
    u8 pad38[0x44 - 0x38];
    u16 lat;
    u16 lon;
    u8 pad48[0x58 - 0x48];
} UnkStruct_02314914_e;

extern UnkStruct_02314914_e *data_ov001_0231f6a4;
extern s16 data_0206e470[];

s32 func_02002278(UnkStruct_02314914_v *a, UnkStruct_02314914_v *b);
void func_02002300(s32 d, UnkStruct_02314914_v *dir, UnkStruct_02314914_v *a, UnkStruct_02314914_v *out);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_02314914(s32 idx, s32 dlat, s16 dlon) {
    UnkStruct_02314914_e *e = &data_ov001_0231f6a4[idx];
    s32 v;
    s32 d;
    UnkStruct_02314914_v dir;

    v = e->lat + dlat;
    e->lon += dlon;
    if (v > 0x7fc0)
        v = 0x7fc0;
    else if (v < 0x40)
        v = 0x40;
    e->lat = v;
    d = func_02002278(&e->pos, &e->target);
    dir.x = FX_Mul(SIN(e->lat), SIN(e->lon));
    dir.y = COS(e->lat);
    dir.z = FX_Mul(SIN(e->lat), COS(e->lon));
    func_02002300(d, &dir, &e->target, &e->pos);
}
