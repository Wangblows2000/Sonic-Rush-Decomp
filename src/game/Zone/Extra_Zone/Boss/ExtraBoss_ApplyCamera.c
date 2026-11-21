#include <nitro/types.h>

typedef struct UnkStruct_022f6f40_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022f6f40_v;

typedef struct UnkStruct_022f6f40 {
    u16 rot;
    u8 pad2[2];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    UnkStruct_022f6f40_v v14;
    UnkStruct_022f6f40_v v20;
    UnkStruct_022f6f40_v v2C;
    UnkStruct_022f6f40_v v38;
} UnkStruct_022f6f40;

typedef struct UnkStruct_022f6f40_c {
    u8 pad0[0xfc];
    u32 flags;
} UnkStruct_022f6f40_c;

extern s16 data_0206e470[];
extern UnkStruct_022f6f40_c NNS_G3dGlb;
extern u8 data_02084d40[];
typedef struct { UnkStruct_022f6f40_v pos, tgt, up; } UnkCam_022f6f40;
extern UnkCam_022f6f40 data_02084f78;
extern u8 data_02084d84[];

void VEC_Add(UnkStruct_022f6f40_v *a, UnkStruct_022f6f40_v *b, UnkStruct_022f6f40_v *out);
void G3i_PerspectiveW_(s32 s, s32 c, s32 x, s32 y, s32 z, s32 a, s32 b, void *out);
void G3i_LookAt_(void *a, void *b, void *c, s32 d, void *e);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void ExtraBoss_ApplyCamera(UnkStruct_022f6f40 *p) {
    UnkStruct_022f6f40_v pos;
    UnkStruct_022f6f40_v tgt;

    VEC_Add(&p->v20, &p->v38, &tgt);
    VEC_Add(&p->v14, &p->v38, &pos);
    G3i_PerspectiveW_(SIN(p->rot), COS(p->rot), p->unkC, p->unk4, p->unk8, p->unk10, 0, data_02084d40);
    NNS_G3dGlb.flags &= ~0x50;
    data_02084f78.pos = pos;
    data_02084f78.tgt = p->v2C;
    data_02084f78.up = tgt;
    G3i_LookAt_(&pos, &p->v2C, &tgt, 0, data_02084d84);
    NNS_G3dGlb.flags &= ~0xe8;
}
