/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_Unk_ov001_022e2f00();
s32 ZoneMap_Unk_ov001_022e2ee8();
s32 ZoneMap_Unk_ov001_022e3180();
s32 ZoneMap_Unk_ov001_022e3840();
s32 ZoneMap_Unk_ov001_022e3844();
void ZoneMap_State_FadeOutExit();

void ZoneMap_Unk_ov001_022e5d08(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0x74) = 0;
    s32 v1 = *(u32 *)t0;
    if (*(u32 *)t0 == 0) {
        s32 t2 = ZoneMap_Unk_ov001_022e2f00(*(u32 *)((u8 *)t0 + 0x4));
        v1 = t2;
    } else {
        s32 t3 = ZoneMap_Unk_ov001_022e2ee8(*(u32 *)((u8 *)t0 + 0x8));
        v1 = t3;
    }
    if (v1 != 0) {
        ZoneMap_Unk_ov001_022e3180(*(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cc4), 0x1e);
    }
    ZoneMap_Unk_ov001_022e3840((*(u32 *)(t0 + 0xa4) + 0x1994), 0x1000);
    ZoneMap_Unk_ov001_022e3844((*(u32 *)(t0 + 0xa4) + 0x1994), 0);
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ZoneMap_State_FadeOutExit;
}

#pragma thumb reset
