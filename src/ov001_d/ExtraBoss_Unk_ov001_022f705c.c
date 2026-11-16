#include <nitro/types.h>

typedef struct UnkStruct_022f705c_v {
    s32 x, y, z;
} UnkStruct_022f705c_v;

typedef struct UnkStruct_022f705c {
    u16 ang;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    UnkStruct_022f705c_v v14;
    UnkStruct_022f705c_v v20;
    UnkStruct_022f705c_v v2C;
    UnkStruct_022f705c_v v38;
} UnkStruct_022f705c;

typedef struct UnkStruct_022f705c_d38 {
    u8 pad[0x4c];
    u8 unk4C[0xfc - 0x4c];
    u32 unkFC;
    u8 pad100[0x240 - 0x100];
    UnkStruct_022f705c_v pos;
    UnkStruct_022f705c_v target;
    UnkStruct_022f705c_v up;
} UnkStruct_022f705c_d38;

extern u8 data_02084d40[];
extern s16 data_0206e470[];
extern UnkStruct_022f705c_d38 NNS_G3dGlb;

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void VEC_Add(const UnkStruct_022f705c_v *a, const UnkStruct_022f705c_v *b, UnkStruct_022f705c_v *c);
s32 FX_Div(s32 n, s32 d);
void G3i_PerspectiveW_(s32 s, s32 c, s32 a, s32 n, s32 f, s32 g, s32 h, void *m);
void G3i_LookAt_(void *a, void *b, void *c, s32 d, void *e);

void ExtraBoss_Unk_ov001_022f705c(UnkStruct_022f705c *p) {
    UnkStruct_022f705c_v pos;
    UnkStruct_022f705c_v up;

    VEC_Add(&p->v20, &p->v38, &up);
    p->v14.z = FX_Div(COS(p->ang), SIN(p->ang)) * 0x60 + p->v20.z;
    VEC_Add(&p->v14, &p->v38, &pos);
    G3i_PerspectiveW_(SIN(p->ang), COS(p->ang), p->unkC, p->unk4, p->unk8, p->unk10, 0, data_02084d40);
    NNS_G3dGlb.unkFC &= ~0x50;
    NNS_G3dGlb.pos = pos;
    NNS_G3dGlb.target = p->v2C;
    NNS_G3dGlb.up = up;
    G3i_LookAt_(&pos, &p->v2C, &up, 0, NNS_G3dGlb.unk4C);
    NNS_G3dGlb.unkFC &= ~0xe8;
}
