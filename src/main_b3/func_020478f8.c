#include <nitro/types.h>

typedef struct { u8 pad0[4]; u8 model[0x134]; } UnkCam_020478f8;
typedef struct {
    u8 pad0[0x10];
    UnkCam_020478f8 cam;  /* 0x10 */
    u8 pad148[0x240];
    void *mdl;            /* 0x388 */
    void *anm38C;         /* 0x38c */
    void *anm390;         /* 0x390 */
} UnkWork_020478f8;
typedef struct { s32 unk0; u8 pad4[0x18]; u8 unk1C; u8 unk1D; } UnkStruct_022c4560;
extern UnkStruct_022c4560 data_022c4560;

void Model_Init(void *o, s32 a);
void Model_SetResource(void *o, void *res, s32 a);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

#pragma thumb on
void func_020478f8(UnkWork_020478f8 *w) {
    UnkCam_020478f8 *cam = &w->cam;

    Model_Init(cam->model, 0);
    Model_SetResource(cam->model, w->mdl, 0);
    if (data_022c4560.unk0 == 0) {
        if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2)
            Model_SetAnimation(cam->model, 0, w->anm38C, 0x27, 0);
        else
            Model_SetAnimation(cam->model, 0, w->anm38C, 0x26, 0);
        Model_SetAnimation(cam->model, 4, w->anm390, 2, 0);
    } else if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2) {
        Model_SetAnimation(cam->model, 0, w->anm38C, 0x49, 0);
        Model_SetAnimation(cam->model, 4, w->anm390, 5, 0);
    } else {
        Model_SetAnimation(cam->model, 0, w->anm38C, 0x48, 0);
        Model_SetAnimation(cam->model, 4, w->anm390, 1, 0);
    }
}
#pragma thumb reset
