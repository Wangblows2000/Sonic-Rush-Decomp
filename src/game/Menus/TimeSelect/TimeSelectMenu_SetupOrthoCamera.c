#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
    s32 z;
} UnkVec_02360268;

typedef struct {
    u8 pad0[0x7C];
    u32 unk7C;
} UnkStruct_02084db8;

extern u8 data_02084d40[];
extern u8 data_02084d84[];
extern UnkStruct_02084db8 data_02084db8;
extern UnkVec_02360268 data_02084f78;
extern UnkVec_02360268 data_02084f84;
extern UnkVec_02360268 data_02084f90;

void G3i_OrthoW_(s32 t, s32 b, s32 l, s32 r, s32 n, s32 f, s32 scaleW, BOOL draw, void *mtx);
void G3i_LookAt_(UnkVec_02360268 *pos, UnkVec_02360268 *up, UnkVec_02360268 *at, s32 a, void *mtx);

#pragma thumb on

void TimeSelectMenu_SetupOrthoCamera(void) {
    UnkVec_02360268 pos;
    UnkVec_02360268 up;
    UnkVec_02360268 at;

    G3i_OrthoW_(0x5f000, -0x60000, -0x80000, 0x7f000, 0x1000, 0xc8000, 0x1000, FALSE, data_02084d40);
    data_02084db8.unk7C &= ~0x50;
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
    up.x = 0;
    up.y = 0x1000;
    up.z = 0;
    at.x = 0;
    at.y = 0;
    at.z = -0x1000;
    data_02084f78 = pos;
    data_02084f84 = up;
    data_02084f90 = at;
    G3i_LookAt_(&pos, &up, &at, 0, data_02084d84);
    data_02084db8.unk7C &= ~0xe8;
}

#pragma thumb reset
