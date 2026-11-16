#include <nitro/types.h>

typedef struct { s32 x; s32 y; s32 z; } UnkVec_022e2af8;

typedef struct {
    UnkVec_022e2af8 pos;
    UnkVec_022e2af8 at;
    UnkVec_022e2af8 up;
} UnkCam_022e2af8;

typedef struct { u8 pad0[0x7c]; u32 unk7C; } UnkStruct_02084db8;

extern UnkVec_022e2af8 data_02084f78;
extern UnkVec_022e2af8 data_02084f84;
extern UnkVec_022e2af8 data_02084f90;
extern u8 data_02084d84[];
extern UnkStruct_02084db8 data_02084db8;

void func_020044f4(UnkVec_022e2af8 *pos, UnkVec_022e2af8 *up, UnkVec_022e2af8 *at, s32 a, void *mtx);

#pragma thumb on
void func_ov001_022e2af8(UnkCam_022e2af8 *c) {
    data_02084f78 = c->pos;
    data_02084f84 = c->up;
    data_02084f90 = c->at;
    func_020044f4(&c->pos, &c->up, &c->at, 0, data_02084d84);
    data_02084db8.unk7C &= ~0xe8;
}
#pragma thumb reset
