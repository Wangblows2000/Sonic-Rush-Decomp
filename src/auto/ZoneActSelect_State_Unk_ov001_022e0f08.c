/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 ZoneActSelect_Unk_ov001_022e0dfc();
s32 ZoneActSelect_Unk_ov001_022e0d30();
s32 ZoneActSelect_Unk_ov001_022e0968();
s32 ZoneActSelect_Unk_ov001_022e08e0();
s32 ZoneActSelect_Unk_ov001_022e09d0();
s32 ZoneActSelect_Unk_ov001_022e0ad0();
s32 ZoneActSelect_Unk_ov001_022e0c5c();
s32 ZoneActSelect_Unk_ov001_022e0884();
extern u8 data_ov001_0231c3ec[];
s32 ZoneActSelect_Unk_ov001_022e0858();
void ZoneActSelect_State_FadeOut();
s32 SysSound_StopBGM();

void ZoneActSelect_State_Unk_ov001_022e0f08(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    ZoneActSelect_Unk_ov001_022e0dfc(*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10));
    ZoneActSelect_Unk_ov001_022e0d30(t0);
    ZoneActSelect_Unk_ov001_022e0968(t0);
    ZoneActSelect_Unk_ov001_022e08e0(t0);
    ZoneActSelect_Unk_ov001_022e09d0(t0);
    ZoneActSelect_Unk_ov001_022e0ad0(t0);
    ZoneActSelect_Unk_ov001_022e0c5c(t0);
    ZoneActSelect_Unk_ov001_022e0dfc(t0);
    ZoneActSelect_Unk_ov001_022e0d30(t0);
    ZoneActSelect_Unk_ov001_022e0884(t0);
    ZoneActSelect_Unk_ov001_022e0858(t0, *(u32 *)data_ov001_0231c3ec);
    *(u32 *)t0 = (*(u32 *)t0 + 0x1);
    *(u32 *)((u8 *)t0 + 0x8) = (*(u32 *)((u8 *)t0 + 0x8) + 0x1);
    *(u32 *)((u8 *)t0 + 0x4) = (*(u32 *)((u8 *)t0 + 0x4) + 0x1);
    if ((u32)*(u32 *)t0 >= (u32)0x3c) {
        *(u32 *)t0 = 0;
        *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)ZoneActSelect_State_FadeOut;
        SysSound_StopBGM(0x1e, (u32)ZoneActSelect_State_FadeOut);
    }
}

#pragma thumb reset
