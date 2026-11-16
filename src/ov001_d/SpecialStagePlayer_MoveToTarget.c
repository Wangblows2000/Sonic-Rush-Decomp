#include <nitro/types.h>

typedef struct { s32 x, y, z; } Vec_02312c18;

typedef struct UnkStruct_02312c18 {
    u8 pad0[0x20];
    Vec_02312c18 pos;
    u8 pad2C[0x44 - 0x2c];
    Vec_02312c18 vel;
    Vec_02312c18 acc;
    u8 pad5C[0x118 - 0x5c];
    struct UnkStruct_02312c18 *target;
} UnkStruct_02312c18;

void VEC_Subtract(const Vec_02312c18 *a, const Vec_02312c18 *b, Vec_02312c18 *c);
s32 VEC_Mag(const Vec_02312c18 *v);
s32 FX_Div(s32 n, s32 d);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void SpecialStagePlayer_MoveToTarget(UnkStruct_02312c18 *p) {
    Vec_02312c18 v;
    s32 mag;

    VEC_Subtract(&p->target->pos, &p->pos, &v);
    mag = VEC_Mag(&v);
    if (mag > 0x4000) {
        v.x = FX_Div(v.x, mag);
        v.y = FX_Div(v.y, mag);
        v.z = FX_Div(v.z, mag);
        v.x = FX_Mul(v.x, 0x4000);
        v.y = FX_Mul(v.y, 0x4000);
        v.z = FX_Mul(v.z, 0x4000);
    }
    p->vel.x = v.x / 4;
    p->vel.y = v.y / 4;
    p->vel.z = v.z / 4;
    p->acc.x = 0;
    p->acc.y = 0;
    p->acc.z = 0;
    p->pos.x += v.x;
    p->pos.y += v.y;
    p->pos.z += v.z;
}
