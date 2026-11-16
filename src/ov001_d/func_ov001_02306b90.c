#include <nitro/types.h>

typedef struct { s32 x, y, z; } Vec_02306b90;

typedef struct UnkStruct_02306b90 {
    u8 pad0[0x20];
    Vec_02306b90 pos;
    Vec_02306b90 vel;
    Vec_02306b90 acc;
    u8 pad44[0x60 - 0x44];
    u16 unk60;
    u8 pad62[0x70 - 0x62];
    s32 timer;
} UnkStruct_02306b90;

s32 VEC_Mag(const Vec_02306b90 *v);

void func_ov001_02306b90(UnkStruct_02306b90 *p) {
    Vec_02306b90 v;

    p->vel.x += p->acc.x;
    p->vel.y += p->acc.y;
    p->vel.z += p->acc.z;
    p->pos.x += p->vel.x;
    p->pos.y += p->vel.y;
    p->pos.z += p->vel.z;
    v.x = p->pos.x;
    v.y = p->pos.y;
    v.z = 0;
    if (VEC_Mag(&v) >= 0x1c000 && p->pos.y <= 0) {
        p->vel.x *= -0.98f;
        p->vel.y *= -0.95f;
        p->pos.x += p->vel.x;
        p->pos.y += p->vel.y;
        p->pos.z += p->vel.z;
    }
    if (p->timer < 0x1e) {
        if (p->timer & 2)
            p->unk60 |= 4;
        else
            p->unk60 &= ~4;
        return;
    }
    if (p->timer < 0x3c) {
        if (p->timer & 4)
            p->unk60 |= 4;
        else
            p->unk60 &= ~4;
    }
}
