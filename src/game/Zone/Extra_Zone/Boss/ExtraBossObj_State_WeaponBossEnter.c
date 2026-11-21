#include <nitro/types.h>

typedef struct UnkStruct_022f62f0 {
    u8 pad0[0x18];
    s32 x;
    s32 y;
    u8 pad20[0xb8 - 0x20];
    u32 flagsB8;
    u8 padBC[0x1c8 - 0xbc];
    s16 t;
} UnkStruct_022f62f0;

void ExtraBoss_WeaponBoss_SelectNextAttack(UnkStruct_022f62f0 *p);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp(s32 t, s32 from, s32 to) {
    s32 v;
    s32 i;
    i = 2;
    v = from;
    do {
        v += FX_Mul(to - v, t);
    } while (i-- != 0);
    return v;
}

void ExtraBossObj_State_WeaponBossEnter(UnkStruct_022f62f0 *p) {
    p->t += 0x22;
    if (p->t >= 0x1000) {
        p->flagsB8 &= ~0x200;
        ExtraBoss_WeaponBoss_SelectNextAttack(p);
        return;
    }
    p->x = Lerp(p->t, 0x1c0000, 0xdc000);
    p->y = Lerp(p->t, 0xc0000, 0xc0000);
}
