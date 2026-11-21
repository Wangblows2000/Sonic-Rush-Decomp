/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjVram_Reset();
s32 Vram_ResetTexAndPltt();
s32 VramPltt_Reset();
s32 Vram_SetBankForBG();
s32 Vram_SetBankForOBJ();
s32 Vram_SetBankForBGExtPltt();
s32 Vram_SetBankForOBJExtPltt();
s32 Vram_SetBankForSubBG();
s32 Vram_SetBankForSubOBJ();
s32 Vram_SetBankForSubBGExtPltt();
s32 Vram_SetBankForSubOBJExtPltt();
s32 Vram_SetBankForTex();
s32 Vram_SetBankForTexPltt();
void VramTex_GfdAlloc();
extern u8 NNS_GfdDefaultFuncAllocTexVram[];
void VramTex_GfdFree();
extern u8 NNS_GfdDefaultFuncFreeTexVram[];
void VramPltt_GfdAlloc();
extern u8 NNS_GfdDefaultFuncAllocPlttVram[];
void VramPltt_GfdFree();
extern u8 NNS_GfdDefaultFuncFreePlttVram[];

void Vram_Init(void) {
    ObjVram_Reset(0);
    ObjVram_Reset(1);
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    Vram_SetBankForBG(0);
    Vram_SetBankForOBJ(0, 0x10, 0x40, 0, 0);
    Vram_SetBankForBGExtPltt(0);
    Vram_SetBankForOBJExtPltt(0);
    Vram_SetBankForSubBG(0);
    Vram_SetBankForSubOBJ(0, 0x10, 0x40, 0, 0);
    Vram_SetBankForSubBGExtPltt(0);
    Vram_SetBankForSubOBJExtPltt(0);
    Vram_SetBankForTex(0);
    Vram_SetBankForTexPltt(0);
    *(u32 *)NNS_GfdDefaultFuncAllocTexVram = (u32)VramTex_GfdAlloc;
    *(u32 *)NNS_GfdDefaultFuncFreeTexVram = (u32)VramTex_GfdFree;
    *(u32 *)NNS_GfdDefaultFuncAllocPlttVram = (u32)VramPltt_GfdAlloc;
    *(u32 *)NNS_GfdDefaultFuncFreePlttVram = (u32)VramPltt_GfdFree;
}
