/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_IsSonicNaviAvailable();
s32 ZoneMap_IsBlazeNaviAvailable();
s32 ZoneMap_SetNaviMessage();
s32 ZoneMap_SetCharModelRotY();
s32 ZoneMap_SetCharModelAnim();
void ZoneMap_State_FadeOutExit();

void ZoneMap_Action_FadeOutExit(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0x74) = 0;
    s32 v1 = *(u32 *)t0;
    if (*(u32 *)t0 == 0) {
        s32 t2 = ZoneMap_IsSonicNaviAvailable(*(u32 *)((u8 *)t0 + 0x4));
        v1 = t2;
    } else {
        s32 t3 = ZoneMap_IsBlazeNaviAvailable(*(u32 *)((u8 *)t0 + 0x8));
        v1 = t3;
    }
    if (v1 != 0) {
        ZoneMap_SetNaviMessage(*(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cc4), 0x1e);
    }
    ZoneMap_SetCharModelRotY((*(u32 *)(t0 + 0xa4) + 0x1994), 0x1000);
    ZoneMap_SetCharModelAnim((*(u32 *)(t0 + 0xa4) + 0x1994), 0);
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ZoneMap_State_FadeOutExit;
}

#pragma thumb reset
