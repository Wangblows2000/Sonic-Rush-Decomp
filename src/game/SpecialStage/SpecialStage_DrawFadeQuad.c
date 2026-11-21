#include <nitro/types.h>

typedef struct UnkStruct_0230b260_v {
    s32 x, y, z;
} UnkStruct_0230b260_v;

typedef struct UnkStruct_0230b260_m33 {
    s32 m[9];
} UnkStruct_0230b260_m33;

typedef struct UnkStruct_0230b260_m43 {
    s32 m[12];
} UnkStruct_0230b260_m43;

typedef struct UnkStruct_0230b260_br {
    u8 pad0[0x20];
    u16 b0 : 1;
    u16 p1 : 5;
    u16 b6 : 2;
    u16 p8 : 1;
    u16 b9 : 1;
    u16 b10 : 1;
    u16 p11 : 1;
    u16 b12 : 1;
    u16 p13 : 3;
    u16 e0 : 5;
    u16 p5 : 3;
    u16 e8 : 5;
    u16 p13b : 3;
} UnkStruct_0230b260_br;

extern UnkStruct_0230b260_br gDisplayMain;

void SpecialStageCamera_GetVectors(void *a, void *b, void *c);
void G3i_LookAt_(void *a, void *b, void *c, s32 d, void *e);
void Model_TransposeRot(void *a, void *o);
void VEC_Normalize(void *a, void *o);
void SpecialStageCamera_Apply(void);
void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);
void SpecialStage_SetPolygonAlpha(void *w);
void SpecialStage_DisableTexture(void);

void SpecialStage_DrawFadeQuad(void *w) {
    u32 color;
    u32 begin;
    u32 v0[2];
    u32 v1[2];
    u32 v2[2];
    u32 v3[2];
    u32 pop;
    UnkStruct_0230b260_v a;
    UnkStruct_0230b260_v b;
    UnkStruct_0230b260_v c;
    UnkStruct_0230b260_v d;
    UnkStruct_0230b260_m43 m43;
    UnkStruct_0230b260_m33 m33;
    u32 tr[3];
    u32 sc[3];

    gDisplayMain.b0 = 1;
    gDisplayMain.b9 = 1;
    gDisplayMain.e0 = 0xf;
    gDisplayMain.b10 = 1;
    gDisplayMain.b12 = 1;
    gDisplayMain.b6 = 1;
    gDisplayMain.e8 = 8;
    SpecialStageCamera_GetVectors(&a, &b, &c);
    G3i_LookAt_(&a, &c, &b, 1, &m43);
    Model_TransposeRot(&m43, &m33);
    d.x = b.x - a.x;
    d.y = b.y - a.y;
    d.z = b.z - a.z;
    VEC_Normalize(&d, &d);
    d.x *= 0x14;
    d.y *= 0x14;
    d.z *= 0x14;
    d.x += a.x;
    d.y += a.y;
    d.z += a.z;
    SpecialStageCamera_Apply();
    NNS_G3dGeBufferOP_N(0x11, NULL, 0);
    SpecialStage_SetPolygonAlpha(w);
    SpecialStage_DisableTexture();
    color = 0x7fff;
    NNS_G3dGeBufferOP_N(0x20, &color, 1);
    tr[0] = d.x;
    tr[1] = d.y;
    tr[2] = d.z;
    NNS_G3dGeBufferOP_N(0x1c, tr, 3);
    NNS_G3dGeBufferOP_N(0x1a, (u32 *)&m33, 9);
    sc[0] = 0x64000;
    sc[1] = 0x64000;
    sc[2] = 0x1000;
    NNS_G3dGeBufferOP_N(0x1b, sc, 3);
    begin = 1;
    NNS_G3dGeBufferOP_N(0x40, &begin, 1);
    v0[0] = 0xf000f000;
    v0[1] = 0;
    NNS_G3dGeBufferOP_N(0x23, v0, 2);
    v1[0] = 0x1000f000;
    v1[1] = 0;
    NNS_G3dGeBufferOP_N(0x23, v1, 2);
    v2[0] = 0x10001000;
    v2[1] = 0;
    NNS_G3dGeBufferOP_N(0x23, v2, 2);
    v3[0] = 0xf0001000;
    v3[1] = 0;
    NNS_G3dGeBufferOP_N(0x23, v3, 2);
    NNS_G3dGeBufferOP_N(0x41, NULL, 0);
    pop = 1;
    NNS_G3dGeBufferOP_N(0x12, &pop, 1);
}
