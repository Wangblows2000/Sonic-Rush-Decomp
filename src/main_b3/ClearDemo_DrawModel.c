#include <nitro/types.h>

typedef struct { s32 x; s32 y; s32 z; } UnkVec_020474f8;
typedef struct {
    u8 pad0[4];
    u8 unk4[0x18];        /* 0x4 */
    s32 unk1C[3];         /* 0x1c */
    u8 unk28[0x24];       /* 0x28 */
    s32 unk4C[3];         /* 0x4c */
    u8 pad58[0xe4];
    s32 unk13C[3];        /* 0x13c */
    s32 unk148[3];        /* 0x148 */
    u16 unk154;           /* 0x154 */
} UnkCam_020474f8;
typedef struct {
    u8 pad0[0x10];
    UnkCam_020474f8 cam;  /* 0x10 */
    u8 pad[0x350 - 0x10 - sizeof(UnkCam_020474f8)];
    u16 unk350;
    u8 pad352[2];
    s32 unk354;
    s32 unk358;
    s32 unk35C;
    u8 pad360[4];
    UnkVec_020474f8 unk364;
} UnkWork_020474f8;

extern s16 data_0206e470[];
extern UnkVec_020474f8 data_02084f78;
extern UnkVec_020474f8 data_02084f84;
extern UnkVec_020474f8 data_02084f90;
extern u8 data_02084d84[];
extern u8 data_02084d40[];
typedef struct { u8 pad0[0x7c]; u32 unk7C; } UnkStruct_02084db8;
extern UnkStruct_02084db8 data_02084db8;

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]
#define COS(a) data_0206e470[(((a) & 0xffff) >> 4) * 2 + 1]

s32 FX_Div(s32 a, s32 b);
void G3i_LookAt_(UnkVec_020474f8 *pos, UnkVec_020474f8 *up, UnkVec_020474f8 *at, s32 a, void *mtx);
void G3i_PerspectiveW_(s32 s, s32 c, s32 a, s32 b, s32 d, s32 e, s32 f, void *mtx);
void MTX_RotY33_(void *mtx, s32 s, s32 c);
void Model_Animate(void *p);
void Model_Draw(void *p);

#define W_VEC(w, off) ((UnkVec_020474f8 *)((u8 *)(w) + (off)))

#pragma thumb on
void ClearDemo_DrawModel(UnkWork_020474f8 *w) {
    UnkCam_020474f8 *cam = &w->cam;

    *(s32 *)((u8 *)w + 0x36c) = FX_Div(COS(w->unk350), SIN(w->unk350)) * 0x60;
    data_02084f78 = *W_VEC(w, 0x364);
    data_02084f84 = *W_VEC(w, 0x37c);
    data_02084f90 = *W_VEC(w, 0x370);
    G3i_LookAt_(W_VEC(w, 0x364), W_VEC(w, 0x37c), W_VEC(w, 0x370), 0, data_02084d84);
    data_02084db8.unk7C &= ~0xe8;
    G3i_PerspectiveW_(SIN(w->unk350), COS(w->unk350), w->unk35C, w->unk354, w->unk358, 0x1000, 0, data_02084d40);
    data_02084db8.unk7C &= ~0x50;
    cam->unk4C[0] = cam->unk148[0];
    cam->unk4C[1] = cam->unk148[1];
    cam->unk4C[2] = cam->unk148[2];
    cam->unk1C[0] = cam->unk13C[0];
    cam->unk1C[1] = cam->unk13C[1];
    cam->unk1C[2] = cam->unk13C[2];
    MTX_RotY33_(cam->unk28, SIN(cam->unk154), COS(cam->unk154));
    Model_Animate(cam->unk4);
    Model_Draw(cam->unk4);
}
#pragma thumb reset
