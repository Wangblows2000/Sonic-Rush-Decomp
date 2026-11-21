#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 w4;
    u16 w6;
    u16 w8;
    u16 wa;
} UnkGfx_020608d8;

extern UnkGfx_020608d8 gDisplayMain;

void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
void TalkDemo_RestoreDisplayZoneMap(void) {
    vu32 z = 0;

    MIi_CpuClear32(z, (void *)0x06002000, 0xa000);
    gDisplayMain.w4 = gDisplayMain.w6 = gDisplayMain.w8 = gDisplayMain.wa = 0;
}
#pragma thumb reset
