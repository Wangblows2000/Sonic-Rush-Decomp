#include <nitro/types.h>

typedef struct UnkStruct_0230efd4_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_0230efd4_v;

typedef struct UnkStruct_0230efd4_t {
    u8 pad0[0xe4];
    UnkStruct_0230efd4_v pos;
} UnkStruct_0230efd4_t;

typedef struct UnkStruct_0230efd4 {
    u8 pad0[0x20];
    UnkStruct_0230efd4_v pos;
} UnkStruct_0230efd4;

UnkStruct_0230efd4_t *SpecialStage_GetPlayer(void);
void VEC_Subtract(UnkStruct_0230efd4_v *a, UnkStruct_0230efd4_v *b, UnkStruct_0230efd4_v *out);
s32 VEC_Mag(UnkStruct_0230efd4_v *v);
s32 func_0200225c(s32 n, s32 d);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

void func_ov001_0230efd4(UnkStruct_0230efd4 *p) {
    UnkStruct_0230efd4_v d;
    s32 len;

    VEC_Subtract(&SpecialStage_GetPlayer()->pos, &p->pos, &d);
    len = VEC_Mag(&d);
    if (len > 0x3000) {
        d.x = func_0200225c(d.x, len);
        d.y = func_0200225c(d.y, len);
        d.z = func_0200225c(d.z, len);
        d.x = FX_Mul(d.x, 0x3000);
        d.y = FX_Mul(d.y, 0x3000);
        d.z = FX_Mul(d.z, 0x3000);
    }
    p->pos.x += d.x;
    p->pos.y += d.y;
    p->pos.z += d.z;
}
