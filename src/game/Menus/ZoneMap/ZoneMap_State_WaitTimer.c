/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_UpdateView();
s32 ZoneMap_SelectNextState();

void ZoneMap_State_WaitTimer(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ZoneMap_UpdateView(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    *(u32 *)((u8 *)t0 + 0x74) = (*(u32 *)((u8 *)t0 + 0x74) + 0x1);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x74);
    if ((u32)t1 >= (u32)*(u32 *)(t0 + 0x80)) {
        *(u32 *)((u8 *)t0 + 0x74) = 0;
        ZoneMap_SelectNextState(0, t1);
    }
}

#pragma thumb reset
