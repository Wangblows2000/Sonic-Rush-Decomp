#include <nitro/types.h>

typedef struct {
    s32 m[4];
} UnkMtx22_022d5a7c;

extern s16 data_0206e470[];
extern u8 data_02087d3c[];

void MTX_Scale22_(UnkMtx22_022d5a7c *m, s32 sx, s32 sy);
void MTX_Rot22_(UnkMtx22_022d5a7c *m, s32 sin, s32 cos);
void MTX_Concat22(const UnkMtx22_022d5a7c *a, const UnkMtx22_022d5a7c *b, UnkMtx22_022d5a7c *ab);
void G2x_SetBGyAffine_(void *dst, UnkMtx22_022d5a7c *m, s32 a, s32 b, s32 c, s32 d);

#pragma thumb on
static inline u32 GetPlaneSub_022d5a7c(void) {
    return (*(vu32 *)0x4001000 & 0x1f00) >> 8;
}

static inline void SetPlaneSub_022d5a7c(u32 plane) {
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | (plane << 8);
}

void func_ov001_022d5a7c(void *p, s32 scale, s32 angle) {
    UnkMtx22_022d5a7c m;
    UnkMtx22_022d5a7c r;
    s32 idx = (angle >> 4) << 1;

    MTX_Scale22_(&m, scale, scale);
    MTX_Rot22_(&r, data_0206e470[(u32)idx], data_0206e470[(u32)(idx + 1)]);
    MTX_Concat22(&m, &r, &m);
    G2x_SetBGyAffine_(data_02087d3c, &m, 0xa8, 0x68, 0, 0);
    SetPlaneSub_022d5a7c(GetPlaneSub_022d5a7c() | 8);
}
#pragma thumb reset
