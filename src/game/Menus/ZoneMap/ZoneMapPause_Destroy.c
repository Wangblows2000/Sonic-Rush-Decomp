/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gZoneMapPauseWork[];
s32 Task_Destroy();
extern u8 data_ov001_0231c588[];

void ZoneMapPause_Destroy(void) {
    s32 t0 = *(u32 *)gZoneMapPauseWork;
    if (t0 != 0) {
        Task_Destroy(*(u32 *)((u8 *)t0 + 0x190), t0);
        *(u32 *)gZoneMapPauseWork = 0;
        *(u32 *)data_ov001_0231c588 = 3;
    }
}

#pragma thumb reset
