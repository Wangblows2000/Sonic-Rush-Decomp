#include <nitro/types.h>

extern u32 gRandSeed;
void TransformDemo_UpdateDualScreen3D(void);
void TransformDemo_SetupDualScreenFrameB(void);
void TransformDemo_SetupDualScreenFrameA(void);
void TransformDemo_SetCameraFrameB(void *p, void *q);
void TransformDemo_SetCameraFrameA(void *p, void *q);
void TransformDemo_SpawnEffect(void *p);
void TransformDemo_Unk_ov001_022df474(void *p);
static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return gRandSeed >> 16;
}

void TransformDemo_UpdateDualScreen3DIntro(u8 *p) {
    *(s16 *)(p + 0xc) -= 1;
    if (*(s16 *)(p + 0xc) <= 0) {
        *(u32 *)(p + 4) |= 0x10;
        *(void (**)(void))p = TransformDemo_UpdateDualScreen3D;
    }
    if (*(u32 *)(p + 4) & 1) {
        TransformDemo_SetupDualScreenFrameB();
        TransformDemo_SetCameraFrameB(p, p + 0x14);
    } else {
        TransformDemo_SetupDualScreenFrameA();
        TransformDemo_SetCameraFrameA(p, p + 0x14);
    }
    *(u32 *)(p + 4) ^= 1;
    if (*(s32 *)(p + 0x10) == 2) {
        if ((Rand() & 7) == 0) {
            TransformDemo_SpawnEffect(p);
        }
    } else {
        if ((Rand() & 3) == 0) {
            TransformDemo_SpawnEffect(p);
        }
    }
    TransformDemo_Unk_ov001_022df474(p);
}
