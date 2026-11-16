#include <nitro/types.h>

extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

s32 func_ov001_022cecb8(void);
void func_ov001_022c8aa0(s32 a, s32 b, s32 c, s32 d);
void func_ov001_022ce164(s32 a);

#pragma thumb on
void func_ov001_022cee8c(void) {
    s32 r = func_ov001_022cecb8();

    func_ov001_022c8aa0(-16, 0, 0x10, 0);
    func_ov001_022ce164(r);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1c00;
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1900;
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset
