#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; u8 unk14; } UnkStruct_02037d60;
extern UnkStruct_02037d60 gTouchState;
extern u8 gTouchSampleBuffer[];
void Touch_StopAuto(void);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

void Touch_Enable(void) {
    vu16 z0, z1;
    if (gTouchState.unk10 & 1) {
        if (!(gTouchState.unk10 & 2)) return;
        Touch_StopAuto();
    }
    z0 = 0;
    MIi_CpuClear16(z0, &gTouchState, 0x28);
    z1 = 0;
    MIi_CpuClear16(z1, gTouchSampleBuffer, 0x28);
    gTouchState.unk14 = 1;
    gTouchState.unk10 |= 1;
}
