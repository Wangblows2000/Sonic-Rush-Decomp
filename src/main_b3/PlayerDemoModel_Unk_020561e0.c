#include <nitro/types.h>

typedef struct { s32 x; s32 y; s32 z; } UnkVec_020561e0;
typedef struct { u8 pad0[0x24]; UnkVec_020561e0 pos; } UnkParam_020561e0;
typedef struct {
    u8 pad0[0x1310];
    UnkVec_020561e0 a;   /* 0x1310 */
    UnkVec_020561e0 b;   /* 0x131c */
    UnkVec_020561e0 up;  /* 0x1328 */
} UnkWork_020561e0;

extern u16 data_022c4674;
extern UnkVec_020561e0 data_02084f78;
extern UnkVec_020561e0 data_02084f84;
extern UnkVec_020561e0 data_02084f90;
extern u8 data_02084d84[];
typedef struct { u8 pad0[0x7c]; u32 unk7C; } UnkStruct_02084db8;
extern UnkStruct_02084db8 data_02084db8;

void NNS_G3dGeBufferOP_N(s32 id, s32 *val, s32 flag);
void NNS_G3dGetCurrentMtx(UnkParam_020561e0 *p, s32 a);
UnkVec_020561e0 *func_02057224(void);
void VEC_Add(const UnkVec_020561e0 *a, const UnkVec_020561e0 *b, UnkVec_020561e0 *ab);
void G3i_LookAt_(UnkVec_020561e0 *pos, UnkVec_020561e0 *up, UnkVec_020561e0 *at, s32 a, void *mtx);

#pragma thumb on
void PlayerDemoModel_Unk_020561e0(UnkWork_020561e0 *w) {
    UnkParam_020561e0 prm = {0};
    UnkVec_020561e0 *a = &w->a;
    UnkVec_020561e0 *up = &w->up;
    UnkVec_020561e0 *b = &w->b;

    if (data_022c4674 & 1) {
        s32 v0;
        s32 v1;
        s32 v2;
        s32 v3;
        v0 = 2;
        NNS_G3dGeBufferOP_N(0x10, &v0, 1);
        v1 = 0x1e;
        NNS_G3dGeBufferOP_N(0x14, &v1, 1);
        NNS_G3dGetCurrentMtx(&prm, 0);
        a->x = prm.pos.x;
        a->y = prm.pos.y;
        a->z = prm.pos.z;
        v2 = 2;
        NNS_G3dGeBufferOP_N(0x10, &v2, 1);
        v3 = 0x1d;
        NNS_G3dGeBufferOP_N(0x14, &v3, 1);
        NNS_G3dGetCurrentMtx(&prm, 0);
        b->x = prm.pos.x;
        b->y = prm.pos.y;
        b->z = prm.pos.z;
        up->x = 0;
        up->y = 0x1000;
        up->z = 0;
        {
            UnkVec_020561e0 off = {0, 0, 0};
            UnkVec_020561e0 pos = {0, 0, 0};
            UnkVec_020561e0 at = {0, 0, 0};
            UnkVec_020561e0 *p = func_02057224();
            if (p != NULL)
                off = *p;
            VEC_Add(b, &off, &at);
            VEC_Add(a, &off, &pos);
            data_02084f78 = pos;
            data_02084f84 = *up;
            data_02084f90 = at;
            G3i_LookAt_(&pos, up, &at, 0, data_02084d84);
            data_02084db8.unk7C &= ~0xe8;
        }
    }
}
#pragma thumb reset
