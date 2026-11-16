/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
s32 Options_CreateFadeTask();
s32 SysSound_PlaySEDelayed();
void func_ov001_022ccbe0();

void func_ov001_022cdca4(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u32 *)((u8 *)t0 + 0x20) = 0;
    *(u32 *)*(u32 *)((u8 *)t0 + 0x3ac) = 8;
    Options_CreateFadeTask(0, -16, 8, 8);
    SysSound_PlaySEDelayed(2, 8, 0x5000, 0x1000);
    *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x8) = (u32)func_ov001_022ccbe0;
}

#pragma thumb reset
