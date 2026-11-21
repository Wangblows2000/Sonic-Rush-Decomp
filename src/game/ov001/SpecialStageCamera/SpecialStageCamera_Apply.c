#include <nitro/types.h>
#pragma opt_propagation off

typedef struct UnkStruct_023157d8_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_023157d8_v;

typedef struct UnkStruct_023157d8_e {
    u8 pad0[0x20];
    UnkStruct_023157d8_v pos;
    UnkStruct_023157d8_v up;
    UnkStruct_023157d8_v target;
    u8 pad44[0x48 - 0x44];
    u16 rot;
    u8 pad4A[2];
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_023157d8_e;

typedef struct UnkStruct_023157d8_g {
    u8 pad0[0x34];
    s32 cur;
} UnkStruct_023157d8_g;

typedef struct UnkStruct_023157d8_c {
    u8 pad0[0xfc];
    u32 flags;
} UnkStruct_023157d8_c;

extern UnkStruct_023157d8_g gSpecialStageCamera;
extern UnkStruct_023157d8_e *gSpecialStageCameraList;
extern s16 data_0206e470[];
extern UnkStruct_023157d8_c NNS_G3dGlb;
extern u8 data_02084d40[];
extern UnkStruct_023157d8_v data_02084f78;
extern UnkStruct_023157d8_v data_02084f84;
extern UnkStruct_023157d8_v data_02084f90;
extern u8 data_02084d84[];

void SpecialStageCamera_LoadProjection(s32 i);
void SpecialStageCamera_LoadLookAt(s32 i);
void G3i_PerspectiveW_(s32 s, s32 c, s32 x, s32 y, s32 z, s32 a, s32 b, void *out);
void G3i_LookAt_(void *a, void *b, void *c, s32 d, void *e);

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

void SpecialStageCamera_Apply(void) {
    UnkStruct_023157d8_e *e = &gSpecialStageCameraList[gSpecialStageCamera.cur];

    SpecialStageCamera_LoadProjection(gSpecialStageCamera.cur);
    SpecialStageCamera_LoadLookAt(gSpecialStageCamera.cur);
    G3i_PerspectiveW_(SIN(e->rot), COS(e->rot), e->x, e->y, e->z, 0x1000, 0, data_02084d40);
    NNS_G3dGlb.flags &= ~0x50;
    data_02084f78 = e->pos;
    data_02084f84 = e->target;
    data_02084f90 = e->up;
    G3i_LookAt_(&e->pos, &e->target, &e->up, 0, data_02084d84);
    NNS_G3dGlb.flags &= ~0xe8;
}
