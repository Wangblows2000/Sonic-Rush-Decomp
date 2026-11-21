/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_IsSonicNaviAvailable();
s32 ZoneMap_IsBlazeNaviAvailable();
s32 ZoneMapPause_Create();
void ZoneMap_State_WaitPauseClosed();

void ZoneMap_Action_OpenPause(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) == 0) {
        ZoneMap_IsSonicNaviAvailable(*(u32 *)((u8 *)t0 + 0x4), t0);
    } else {
        ZoneMap_IsBlazeNaviAvailable(*(u32 *)((u8 *)t0 + 0x8));
    }
    ZoneMapPause_Create();
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ZoneMap_State_WaitPauseClosed;
}

#pragma thumb reset
