#include <nitro/types.h>

typedef struct UnkStruct_022fac54 {
    u8 pad0[0x50];
    s32 vx;
    s32 vy;
    u8 pad58[0x74 - 0x58];
    void (*state)(struct UnkStruct_022fac54 *p);
    u8 pad78[0x1cc - 0x78];
    union {
        s16 t;
        s32 v;
    } unk1CC;
    u8 pad1D0[4];
    s32 tx;
    s32 ty;
} UnkStruct_022fac54;

void ExtraBossObj_State_MissileHoming(UnkStruct_022fac54 *p);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Approach(s32 t, s32 target) {
    s32 v;
    s32 i;
    i = 2;
    v = 0;
    do {
        v = target + FX_Mul(v - target, t);
    } while (i-- != 0);
    return v;
}

void ExtraBossObj_State_MissileLaunch(UnkStruct_022fac54 *p) {
    p->unk1CC.t += 0x44;
    if (p->unk1CC.v >= 0x1000) {
        p->state = ExtraBossObj_State_MissileHoming;
        return;
    }
    p->vx = Approach(p->unk1CC.t, p->tx);
    p->vy = Approach(p->unk1CC.t, p->ty);
}
