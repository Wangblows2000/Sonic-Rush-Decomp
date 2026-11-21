#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u16 unk4;
    s16 t;
    s32 unk8;
    s32 unkC;
} UnkObj_02045df0;
typedef struct {
    u8 pad0[0x344];
    u32 flags;
} UnkWork_02045df0;

void ClearDemoBars_State_Done(void);

#pragma thumb on
static inline s32 FX_Mul_02045df0(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

static inline s32 Lerp_02045df0(s32 v, s32 to, s32 t, s32 n) {
    do {
        v += FX_Mul_02045df0(to - v, t);
    } while (n--);
    return v;
}

void ClearDemoBars_State_SlideIn(UnkWork_02045df0 *w, UnkObj_02045df0 *o) {
    s32 v;

    o->unk4++;
    if (o->unk4 >= 0x10) {
        o->fn = ClearDemoBars_State_Done;
        o->unk4 = 0;
        o->unk8 = 0;
        o->unkC = 0xc0000;
        w->flags |= 0x80000;
        return;
    }
    o->t += 0x100;
    if (o->t > 0x1000)
        o->t = 0x1000;
    v = Lerp_02045df0(0, 0x10000, o->t, 5);
    o->unk8 = -(0x10000 - v);
    o->unkC = 0xd0000 - v;
}
#pragma thumb reset
