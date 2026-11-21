/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_UpdateView();
s32 ZoneMapPause_IsClosed();
s32 ZoneMap_Unk_ov001_022e955c();
s32 ZoneMap_SelectNextState();
s32 ZoneMap_Action_FadeOutExit();

void ZoneMap_State_WaitPauseClosed(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ZoneMap_UpdateView(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    s32 t1 = (*(u32 *)((u8 *)t0 + 0x74) + 0x1);
    *(u32 *)((u8 *)t0 + 0x74) = t1;
    s32 t2 = ZoneMapPause_IsClosed(t1);
    if (t2 != 0) {
        s32 t3 = ZoneMap_Unk_ov001_022e955c();
        if (t3 == 0) {
            ZoneMap_SelectNextState();
            return;
        }
        ZoneMap_Action_FadeOutExit();
    }
}

#pragma thumb reset
