/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
extern u8 gDisplayMain[];
s32 SysSound_PlaySE();
void ZoneMapPause_State_Open();

void ZoneMapPause_Unk_ov001_022e926c(s32 p0) {
    s32 v0 = (s32)p0;
    if ((s32)p0 == 0) {
        v0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    }
    *(u32 *)v0 = 1;
    *(u32 *)((u8 *)v0 + 0x4) = 0;
    *(u32 *)((u8 *)v0 + 0x10) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0x8) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0xa) = 0x48;
    *(u32 *)((u8 *)v0 + 0xc) = 0x48;
    SysSound_PlaySE(0xa, 0x48);
    *(u32 *)((u8 *)*(u32 *)((u8 *)v0 + 0x190) + 0x8) = (u32)ZoneMapPause_State_Open;
}

#pragma thumb reset
