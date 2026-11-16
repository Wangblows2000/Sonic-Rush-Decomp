/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_Unk_ov001_022e6dec();
s32 ZoneMap_Unk_ov001_022e6870();

void ZoneMap_State_Unk_ov001_022e6e9c(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ZoneMap_Unk_ov001_022e6dec(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    *(u32 *)((u8 *)t0 + 0x74) = (*(u32 *)((u8 *)t0 + 0x74) + 0x1);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x74);
    if ((u32)t1 >= (u32)*(u32 *)(t0 + 0x80)) {
        *(u32 *)((u8 *)t0 + 0x74) = 0;
        ZoneMap_Unk_ov001_022e6870(0, t1);
    }
}

#pragma thumb reset
