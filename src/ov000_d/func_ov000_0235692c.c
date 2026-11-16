#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_0235692c;

typedef struct {
    UnkVec_0235692c pos;
    UnkVec_0235692c up;
    UnkVec_0235692c at;
} UnkCam_0235692c;

typedef struct {
    u8 pad0[0x7C];
    u32 unk7C;
} UnkStruct_02084db8;

extern UnkVec_0235692c data_02084f78;
extern UnkVec_0235692c data_02084f84;
extern UnkVec_0235692c data_02084f90;
extern u8 data_02084d84[];
extern UnkStruct_02084db8 data_02084db8;

void func_020044f4(UnkVec_0235692c *pos, UnkVec_0235692c *up, UnkVec_0235692c *at, s32 a, void *mtx);

#pragma thumb on

void func_ov000_0235692c(UnkCam_0235692c *cam) {
    data_02084f78 = cam->pos;
    data_02084f84 = cam->up;
    data_02084f90 = cam->at;
    func_020044f4(&cam->pos, &cam->up, &cam->at, 0, data_02084d84);
    data_02084db8.unk7C &= ~0xe8;
}

#pragma thumb reset
