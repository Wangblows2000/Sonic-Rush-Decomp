/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_Unk_ov001_022e2f00();
s32 ZoneMap_Unk_ov001_022e2ee8();
s32 ZoneMapPause_Create();
void ZoneMap_State_Unk_ov001_022e76ec();

void ZoneMap_Unk_ov001_022e5e60(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    if (*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) == 0) {
        ZoneMap_Unk_ov001_022e2f00(*(u32 *)((u8 *)t0 + 0x4), t0);
    } else {
        ZoneMap_Unk_ov001_022e2ee8(*(u32 *)((u8 *)t0 + 0x8));
    }
    ZoneMapPause_Create();
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ZoneMap_State_Unk_ov001_022e76ec;
}

#pragma thumb reset
