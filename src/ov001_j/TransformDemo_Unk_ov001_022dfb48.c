#pragma opt_unroll_loops off
#include <nitro/types.h>

typedef struct UnkStruct_022dfb48_v {
    s32 x, y, z;
} UnkStruct_022dfb48_v;

typedef struct UnkStruct_022dfb48_c {
    u8 pad0[0x14];
    UnkStruct_022dfb48_v pos;
    UnkStruct_022dfb48_v up;
    UnkStruct_022dfb48_v target;
    s32 unk38;
    s32 unk3C;
} UnkStruct_022dfb48_c;

typedef struct UnkStruct_022dfb48_w {
    u8 pad0[0xbd8];
    s32 unkBD8;
} UnkStruct_022dfb48_w;

typedef struct UnkStruct_022dfb48_d38 {
    u8 pad0[0x4c];
    u8 unk4C[0xfc - 0x4c];
    u32 unkFC;
    u8 pad100[0x240 - 0x100];
    UnkStruct_022dfb48_v pos;
    UnkStruct_022dfb48_v target;
    UnkStruct_022dfb48_v up;
} UnkStruct_022dfb48_d38;

extern UnkStruct_022dfb48_v data_02084f78;
extern UnkStruct_022dfb48_v data_02084f84;
extern UnkStruct_022dfb48_v data_02084f90;
extern u8 data_02084d84[];
extern UnkStruct_022dfb48_d38 NNS_G3dGlb;

s32 FX_Div(s32 n, s32 d);
void G3i_LookAt_(void *a, void *b, void *c, s32 d, void *e);

void TransformDemo_Unk_ov001_022dfb48(UnkStruct_022dfb48_w *w, UnkStruct_022dfb48_c *c) {
    s64 k = c->unk38;
    s32 lim = c->unk3C;
    s32 v = w->unkBD8;
    s32 a;
    s32 b;

    a = FX_Div((s32)((v * k + 0x800) >> 12), 0x19000);
    b = FX_Div(v - (s32)((k * 0x19000 + 0x800) >> 12), a + 0x1000);
    if (v > lim) {
        c->up.y = b;
    } else if (v > -lim) {
        c->up.y = -lim;
    } else {
        c->up.y = v;
    }
    NNS_G3dGlb.pos = c->pos;
    NNS_G3dGlb.target = c->target;
    NNS_G3dGlb.up = c->up;
    G3i_LookAt_(&c->pos, &c->target, &c->up, 0, NNS_G3dGlb.unk4C);
    NNS_G3dGlb.unkFC &= ~0xe8;
}
