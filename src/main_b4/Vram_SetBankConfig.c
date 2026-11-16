#include <nitro/types.h>

void Vram_SetBankForBG(s32 a);
void Vram_SetBankForOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForBGExtPltt(s32 a);
void Vram_SetBankForOBJExtPltt(s32 a);
void Vram_SetBankForTex(s32 a);
void Vram_SetBankForTexPltt(s32 a);
void func_02002ec8(void);
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForSubBGExtPltt(s32 a);
void Vram_SetBankForSubOBJExtPltt(s32 a);
void func_02002eb4(void);
void func_02002ea0(void);
void ObjVram_Reset(s32 i);

void Vram_SetBankConfig(s32 mode) {
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
    switch (mode) {
    case 0:
        Vram_SetBankForBG(1);
        Vram_SetBankForOBJ(2, 0x200010, 0x40, 0, 0x400);
        Vram_SetBankForBGExtPltt(0x10);
        Vram_SetBankForOBJExtPltt(0x20);
        ObjVram_Reset(0);
        Vram_SetBankForSubBG(4);
        Vram_SetBankForSubOBJ(8, 0x200010, 0x40, 0, 0x400);
        Vram_SetBankForSubBGExtPltt(0x80);
        Vram_SetBankForSubOBJExtPltt(0x100);
        ObjVram_Reset(1);
        break;
    case 1:
        Vram_SetBankForTex(7);
        Vram_SetBankForTexPltt(0x10);
        Vram_SetBankForBG(8);
        Vram_SetBankForOBJ(0x60, 0x10, 0x40, 0, 0x400);
        ObjVram_Reset(0);
        Vram_SetBankForSubBG(0x80);
        Vram_SetBankForSubOBJ(0x100, 0x10, 0x40, 0, 0x200);
        ObjVram_Reset(1);
        break;
    case 2:
        Vram_SetBankForTex(0xf);
        Vram_SetBankForTexPltt(0x70);
        Vram_SetBankForOBJ(0, 0x10, 0x40, 0, 0);
        Vram_SetBankForSubBG(0x80);
        Vram_SetBankForSubOBJ(0x100, 0x10, 0x40, 0, 0x200);
        ObjVram_Reset(1);
        break;
    }
}
