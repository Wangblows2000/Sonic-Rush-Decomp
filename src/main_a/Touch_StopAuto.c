#include <nitro/types.h>

typedef struct { u8 unk0[0x10]; u32 unk10; u8 unk14; } UnkStruct_02037cac;
extern UnkStruct_02037cac gTouchState;
extern u32 gSystemFlags;
void TP_RequestAutoSamplingStopAsync(void);
void TP_WaitBusy(u32 a);
BOOL TP_CheckError(u32 a);

void Touch_StopAuto(void) {
    if (gTouchState.unk10 & 2) {
        TP_RequestAutoSamplingStopAsync();
        TP_WaitBusy(4);
        if (TP_CheckError(4)) return;
        gTouchState.unk10 &= ~1;
        gTouchState.unk10 &= ~2;
        gTouchState.unk14 = 0;
        gSystemFlags &= ~0x8000;
    } else {
        gTouchState.unk10 &= ~1;
    }
}
