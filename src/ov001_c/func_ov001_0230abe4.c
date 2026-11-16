#include <nitro/types.h>

typedef struct UnkStruct_0230abe4_v {
    s32 x, y, z;
} UnkStruct_0230abe4_v;

typedef struct UnkStruct_0230abe4_c {
    s32 x;
    s32 y;
    s32 z;
    s32 r;
} UnkStruct_0230abe4_c;

extern s16 data_0206e470[];

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

s32 func_02002278(const UnkStruct_0230abe4_v *a, const UnkStruct_0230abe4_v *b);

static inline s32 FX_Mul(s32 a, s32 b) {
    return (s32)(((s64)a * b + 0x800) >> 12);
}

BOOL func_ov001_0230abe4(UnkStruct_0230abe4_c *a, UnkStruct_0230abe4_v *pa, u16 angA, UnkStruct_0230abe4_c *b, UnkStruct_0230abe4_v *pb, u16 angB) {
    UnkStruct_0230abe4_v va = *pa;
    UnkStruct_0230abe4_v vb = *pb;
    s32 idx;
    s32 c;
    s32 s;

    idx = (s32)(angA & 0xffff) >> 4;
    c = COS_IDX(idx);
    s = SIN_IDX(idx);
    va.x += FX_Mul(a->x, c);
    va.y += FX_Mul(a->x, s);
    va.x -= FX_Mul(a->y, s);
    va.y += FX_Mul(a->y, c);
    va.z += a->z;
    idx = (s32)(angB & 0xffff) >> 4;
    c = COS_IDX(idx);
    s = SIN_IDX(idx);
    vb.x += FX_Mul(b->x, c);
    vb.y += FX_Mul(b->x, s);
    vb.x -= FX_Mul(b->y, s);
    vb.y += FX_Mul(b->y, c);
    vb.z += b->z;
    return func_02002278(&va, &vb) <= a->r + b->r;
}
