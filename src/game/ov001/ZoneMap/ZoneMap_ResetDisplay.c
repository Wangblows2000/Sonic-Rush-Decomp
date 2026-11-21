/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 GX_DisableBankForBG();
s32 GX_DisableBankForOBJ();
s32 GX_DisableBankForBGExtPltt();
s32 GX_DisableBankForOBJExtPltt();
s32 GX_DisableBankForTex();
s32 GX_DisableBankForTexPltt();
s32 GX_DisableBankForClearImage();
s32 GX_DisableBankForSubBG();
s32 GX_DisableBankForSubOBJ();
s32 GX_DisableBankForSubBGExtPltt();
s32 GX_DisableBankForSubOBJExtPltt();
s32 GX_DisableBankForARM7();
s32 GX_DisableBankForLCDC();
s32 Vram_SetBankForBG();
s32 Vram_SetBankForOBJ();
s32 Vram_SetBankForBGExtPltt();
s32 Vram_SetBankForOBJExtPltt();
s32 Vram_SetBankForTex();
s32 Vram_SetBankForTexPltt();
s32 Vram_SetBankForSubBG();
s32 Vram_SetBankForSubOBJ();
s32 Vram_SetBankForSubBGExtPltt();
s32 Vram_SetBankForSubOBJExtPltt();
s32 Vram_ResetTexAndPltt();
s32 VramPltt_Reset();
extern u8 gDisplaySub[];
extern u8 gDisplayMain[];

void ZoneMap_ResetDisplay(void) {
    *(volatile u16 *)0x5000000 = 0xffff;
    *(volatile u16 *)0x5000400 = 0xffff;
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & -7937);
    *(volatile u32 *)0x4001000 = (*(volatile u32 *)0x4001000 & -7937);
    *(volatile u16 *)0x4000304 = ((*(volatile u16 *)0x4000304 & -527) | 0x20e);
    s32 t0 = (*(volatile u16 *)0x4000304 | 0x8000);
    *(volatile u16 *)0x4000304 = t0;
    GX_DisableBankForBG(0x8000, t0, 0x4000304);
    GX_DisableBankForOBJ();
    GX_DisableBankForBGExtPltt();
    GX_DisableBankForOBJExtPltt();
    GX_DisableBankForTex();
    GX_DisableBankForTexPltt();
    GX_DisableBankForClearImage();
    GX_DisableBankForSubBG();
    GX_DisableBankForSubOBJ();
    GX_DisableBankForSubBGExtPltt();
    GX_DisableBankForSubOBJExtPltt();
    GX_DisableBankForARM7();
    GX_DisableBankForLCDC();
    Vram_SetBankForBG(0);
    Vram_SetBankForOBJ(0, 0x10, 0x40, 0, 0);
    Vram_SetBankForBGExtPltt(0);
    Vram_SetBankForOBJExtPltt(0);
    Vram_SetBankForTex(0);
    Vram_SetBankForTexPltt(0);
    GX_DisableBankForClearImage();
    Vram_SetBankForSubBG(0);
    Vram_SetBankForSubOBJ(0, 0x10, 0x40, 0, 0);
    Vram_SetBankForSubBGExtPltt(0);
    Vram_SetBankForSubOBJExtPltt(0);
    GX_DisableBankForARM7();
    GX_DisableBankForLCDC();
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    *(u16 *)((u8 *)gDisplaySub + 0xe) = 0;
    *(u16 *)((u8 *)gDisplaySub + 0xc) = *(u16 *)((u8 *)gDisplaySub + 0xe);
    *(u16 *)((u8 *)gDisplaySub + 0xa) = *(u16 *)((u8 *)gDisplaySub + 0xc);
    *(u16 *)((u8 *)gDisplaySub + 0x8) = *(u16 *)((u8 *)gDisplaySub + 0xa);
    *(u16 *)((u8 *)gDisplaySub + 0x6) = *(u16 *)((u8 *)gDisplaySub + 0x8);
    *(u16 *)((u8 *)gDisplaySub + 0x4) = *(u16 *)((u8 *)gDisplaySub + 0x6);
    *(u16 *)((u8 *)gDisplaySub + 0x2) = *(u16 *)((u8 *)gDisplaySub + 0x4);
    *(u16 *)gDisplaySub = *(u16 *)((u8 *)gDisplaySub + 0x2);
    *(u16 *)((u8 *)gDisplayMain + 0xe) = *(u16 *)gDisplaySub;
    *(u16 *)((u8 *)gDisplayMain + 0xc) = *(u16 *)((u8 *)gDisplayMain + 0xe);
    *(u16 *)((u8 *)gDisplayMain + 0xa) = *(u16 *)((u8 *)gDisplayMain + 0xc);
    *(u16 *)((u8 *)gDisplayMain + 0x8) = *(u16 *)((u8 *)gDisplayMain + 0xa);
    *(u16 *)((u8 *)gDisplayMain + 0x6) = *(u16 *)((u8 *)gDisplayMain + 0x8);
    *(u16 *)((u8 *)gDisplayMain + 0x4) = *(u16 *)((u8 *)gDisplayMain + 0x6);
    *(u16 *)((u8 *)gDisplayMain + 0x2) = *(u16 *)((u8 *)gDisplayMain + 0x4);
    *(u16 *)gDisplayMain = *(u16 *)((u8 *)gDisplayMain + 0x2);
}

#pragma thumb reset
