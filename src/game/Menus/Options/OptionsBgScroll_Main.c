/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gTaskCurrent[];
extern u8 gDisplayMain[];
extern u8 gDisplaySub[];

void OptionsBgScroll_Main(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10);
    *(u16 *)((u8 *)gDisplayMain + 0xc) = ((s32)*(u32 *)*(u32 *)((u8 *)*(u32 *)gTaskCurrent + 0x10) >> 0xc);
    *(u16 *)((u8 *)gDisplayMain + 0xe) = ((s32)*(u32 *)((u8 *)t0 + 0x4) >> 0xc);
    *(u16 *)((u8 *)gDisplaySub + 0xc) = ((s32)*(u32 *)t0 >> 0xc);
    *(u16 *)((u8 *)gDisplaySub + 0xe) = ((s32)*(u32 *)((u8 *)t0 + 0x4) >> 0xc);
    *(u32 *)t0 = (*(u32 *)t0 + 0x2000);
}

#pragma thumb reset
