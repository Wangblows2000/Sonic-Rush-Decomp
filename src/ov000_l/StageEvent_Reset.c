#include <nitro/types.h>

extern u8 gStageEventFiles[];
extern void *data_ov000_0236aa8c;
extern void *gStageEventManagerTask;
extern u16 data_ov000_0236aa78;
extern u16 data_ov000_0236aa7c;
extern u16 data_ov000_0236aa80;

void MIi_CpuClear16(u16 data, void *dest, u32 size);

void StageEvent_Reset(void) {
    vu16 zero = 0;

    MIi_CpuClear16(zero, gStageEventFiles, 0xc);
    data_ov000_0236aa8c = NULL;
    gStageEventManagerTask = NULL;
    data_ov000_0236aa78 = 0;
    data_ov000_0236aa7c = 0;
    data_ov000_0236aa80 = 0;
}
