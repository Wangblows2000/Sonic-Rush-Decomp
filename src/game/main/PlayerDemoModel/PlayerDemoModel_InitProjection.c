#include <nitro/types.h>

typedef struct {
    u16 angle;
    u16 pad2;
    s32 n;
    s32 f;
    s32 aspect;
    s32 scale;
} UnkPersp_02056328;

typedef struct {
    u8 pad0[0x12fc];
    UnkPersp_02056328 persp;
} UnkWork_02056328;

typedef struct {
    u8 pad0[0x7c];
    u32 flags;
} UnkG3_02056328;

extern s16 data_0206e470[];
extern u8 data_02084d40[];
extern UnkG3_02056328 data_02084db8;

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void G3i_PerspectiveW_(s32 s, s32 c, s32 aspect, s32 n, s32 f, s32 scale, s32 z, void *mtx);

#pragma thumb on
void PlayerDemoModel_InitProjection(UnkWork_02056328 *w) {
    UnkPersp_02056328 *p = &w->persp;
    s32 idx;

    p->angle = 0x11c7;
    p->n = 0x1000;
    p->f = 0x800000;
    p->aspect = 0x1555;
    p->scale = 0x1000;
    idx = (s32)(p->angle & 0xffff) >> 4;
    G3i_PerspectiveW_(SIN_IDX(idx), COS_IDX(idx), p->aspect, p->n, p->f, p->scale, 0, data_02084d40);
    data_02084db8.flags &= ~0x50;
}
#pragma thumb reset
