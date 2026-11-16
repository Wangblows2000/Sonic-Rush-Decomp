/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneMap_Unk_ov001_022e5618();
s32 ZoneMap_Unk_ov001_022e2f00();
s32 ZoneMap_Unk_ov001_022e3180();
s32 SysSound_StopBGM();
void ZoneMap_State_PlayTalkDemo();

void ZoneMap_Unk_ov001_022e5f04(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ZoneMap_Unk_ov001_022e5618((*(u32 *)(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) + 0xa4) + 0x4), 0);
    s32 t1 = ZoneMap_Unk_ov001_022e2f00(*(u32 *)((u8 *)t0 + 0x4));
    if (t1 != 0) {
        ZoneMap_Unk_ov001_022e3180(*(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cc4), 2);
    } else {
        ZoneMap_Unk_ov001_022e3180(*(u32 *)((u8 *)*(u32 *)(t0 + 0xa4) + 0x1cc4), 0);
    }
    *(u32 *)(t0 + 0x84) = 0;
    *(u32 *)(t0 + 0x88) = 0;
    *(u32 *)(t0 + 0x8c) = 0;
    SysSound_StopBGM(0xf, 0);
    *(u32 *)((u8 *)t0 + 0x74) = 0;
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ZoneMap_State_PlayTalkDemo;
}

#pragma thumb reset
