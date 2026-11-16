#include <nitro/types.h>

void PXI_Init(void);
void TP_Init(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void TP_SetCallback(void *cb);
BOOL TP_GetUserInfo(void *p);
void TP_SetCalibrateParam(void *p);
void Touch_NullCallback(void);
extern u8 gTouchState[];
extern u8 gTouchSampleBuffer[];

void Touch_Init(void) {
    u8 buf[8];
    vu16 z0, z1;
    PXI_Init();
    TP_Init();
    z0 = 0;
    MIi_CpuClear16(z0, gTouchState, 0x28);
    z1 = 0;
    MIi_CpuClear16(z1, gTouchSampleBuffer, 0x28);
    TP_SetCallback(Touch_NullCallback);
    if (!TP_GetUserInfo(buf)) return;
    TP_SetCalibrateParam(buf);
}
