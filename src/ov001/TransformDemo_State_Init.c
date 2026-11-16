#include <nitro/types.h>

void TransformDemo_InitDisplay(void *p);
void TransformDemo_LoadPlayerArchive(void *p, void *q);
void TransformDemo_LoadPlayerModels(void *p, void *q);
void TransformDemo_SetupLights(void *p);
void TransformDemo_Unk_ov001_022dfc54(void *p, void *q);
void TransformDemo_InitCaptureOam(void *p);
void TransformDemo_UpdateDualScreen3DIntro(void);

void TransformDemo_State_Init(u8 *p) {
    TransformDemo_InitDisplay(p);
    TransformDemo_LoadPlayerArchive(p, p + 0x454);
    TransformDemo_LoadPlayerModels(p, p + 0xbd4);
    TransformDemo_SetupLights(p);
    TransformDemo_Unk_ov001_022dfc54(p, p + 0x14);
    TransformDemo_InitCaptureOam(p);
    *(u16 *)(p + 0xc) = 0x1e;
    *(void (**)(void))p = TransformDemo_UpdateDualScreen3DIntro;
}
