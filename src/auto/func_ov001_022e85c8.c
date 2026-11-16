/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02002f60();
s32 func_02002f4c();
s32 func_02002f28();
s32 func_02002f04();
s32 func_02002ef0();
s32 func_02002edc();
s32 func_02002ec8();
s32 func_02002e8c();
s32 func_02002e78();
s32 func_02002e50();
s32 func_02002e28();
s32 func_02002eb4();
s32 func_02002ea0();
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

void func_ov001_022e85c8(void) {
    *(volatile u16 *)0x5000000 = 0xffff;
    *(volatile u16 *)0x5000400 = 0xffff;
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & -7937);
    *(volatile u32 *)0x4001000 = (*(volatile u32 *)0x4001000 & -7937);
    *(volatile u16 *)0x4000304 = ((*(volatile u16 *)0x4000304 & -527) | 0x20e);
    s32 t0 = (*(volatile u16 *)0x4000304 | 0x8000);
    *(volatile u16 *)0x4000304 = t0;
    func_02002f60(0x8000, t0, 0x4000304);
    func_02002f4c();
    func_02002f28();
    func_02002f04();
    func_02002ef0();
    func_02002edc();
    func_02002ec8();
    func_02002e8c();
    func_02002e78();
    func_02002e50();
    func_02002e28();
    func_02002eb4();
    func_02002ea0();
    Vram_SetBankForBG(0);
    Vram_SetBankForOBJ(0, 0x10, 0x40, 0, 0);
    Vram_SetBankForBGExtPltt(0);
    Vram_SetBankForOBJExtPltt(0);
    Vram_SetBankForTex(0);
    Vram_SetBankForTexPltt(0);
    func_02002ec8();
    Vram_SetBankForSubBG(0);
    Vram_SetBankForSubOBJ(0, 0x10, 0x40, 0, 0);
    Vram_SetBankForSubBGExtPltt(0);
    Vram_SetBankForSubOBJExtPltt(0);
    func_02002eb4();
    func_02002ea0();
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
