/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 Vram_DisableAllBanks();
extern u8 gDisplayMain[];
extern u8 gDisplaySub[];

void Options_Unk_ov001_022ce304(void) {
    *(volatile u16 *)0x5000000 = 0xffff;
    *(volatile u16 *)0x5000400 = 0xffff;
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & -7937);
    s32 t0 = (*(volatile u32 *)0x4001000 & -7937);
    *(volatile u32 *)0x4001000 = t0;
    Vram_DisableAllBanks(-7937, 0x4001000, t0);
    *(u16 *)gDisplayMain = 0;
    *(u16 *)((u8 *)gDisplayMain + 0x2) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0x4) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0x6) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0x8) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0xa) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0xc) = 0;
    *(u16 *)((u8 *)gDisplayMain + 0xe) = 0;
    *(u16 *)gDisplaySub = 0;
    *(u16 *)((u8 *)gDisplaySub + 0x2) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0x4) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0x6) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0x8) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0xa) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0xc) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0xe) = 0;
}

#pragma thumb reset
