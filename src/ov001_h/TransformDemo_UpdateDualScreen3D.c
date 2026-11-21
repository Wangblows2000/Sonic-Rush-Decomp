#include <nitro/types.h>

typedef struct UnkStruct_022e023c {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[0x10 - 8];
    s32 unk10;
    u8 cam[0x454 - 0x14];
    u8 unk454[0xbd4 - 0x454];
    struct {
        u8 unk0[0x10];
    } unkBD4;
} UnkStruct_022e023c;

extern u32 gRandSeed;

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void TransformDemo_SetupDualScreenFrameB(void);
void TransformDemo_SetupDualScreenFrameA(void);
void TransformDemo_SetCameraFrameB(UnkStruct_022e023c *w, void *cam);
void TransformDemo_SetCameraFrameA(UnkStruct_022e023c *w, void *cam);
void TransformDemo_SpawnEffect(UnkStruct_022e023c *w);
void TransformDemo_DrawModels(UnkStruct_022e023c *w, void *a);
void TransformDemo_UpdatePlayerModels(UnkStruct_022e023c *w, void *a);
void TransformDemo_DrawPlayerModels(UnkStruct_022e023c *w, void *a);
void TransformDemo_CopyCaptureOam(UnkStruct_022e023c *w);

void TransformDemo_UpdateDualScreen3D(UnkStruct_022e023c *w) {
    if (w->unk4 & 1) {
        TransformDemo_SetupDualScreenFrameB();
        TransformDemo_SetCameraFrameB(w, w->cam);
    } else {
        TransformDemo_SetupDualScreenFrameA();
        TransformDemo_SetCameraFrameA(w, w->cam);
    }
    w->unk4 ^= 1;
    if (w->unk10 == 2) {
        if ((Rand() & 7) == 0)
            TransformDemo_SpawnEffect(w);
    } else {
        if ((Rand() & 3) == 0)
            TransformDemo_SpawnEffect(w);
    }
    TransformDemo_DrawModels(w, w->unk454);
    TransformDemo_UpdatePlayerModels(w, &w->unkBD4);
    TransformDemo_DrawPlayerModels(w, w->unkBD4.unk0);
    TransformDemo_CopyCaptureOam(w);
}
