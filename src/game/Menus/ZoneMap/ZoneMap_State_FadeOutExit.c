/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_CreateFadeTask();
s32 SysSound_StopBGM();
s32 ZoneMap_UpdateView();

void ZoneMap_State_FadeOutExit(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (*(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0x74) == 0xa4) {
        ZoneMap_CreateFadeTask(0, 0x10, 0x10, 0, 1, 1);
        SysSound_StopBGM(0xf);
    }
    ZoneMap_UpdateView(t0);
    *(u32 *)((u8 *)t0 + 0x74) = (*(u32 *)((u8 *)t0 + 0x74) + 0x1);
    if ((u32)*(u32 *)((u8 *)t0 + 0x74) >= (u32)0xb4) {
        *(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cc8) = 3;
        *(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1ccc) = -1;
        *(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cd0) = -1;
        *(u32 *)*(u32 *)(t0 + 0xa4) = 1;
    }
}

#pragma thumb reset
