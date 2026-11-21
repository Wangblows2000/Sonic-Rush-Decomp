#include <nitro/types.h>

typedef struct UnkStruct_022c634c_br {
    u8 pad0[0x48];
    s16 unk48;
} UnkStruct_022c634c_br;

typedef struct UnkStruct_022c634c_f754 {
    u8 pad[0x80];
    vu32 unk80;
    vu32 unk84;
} UnkStruct_022c634c_f754;

extern UnkStruct_022c634c_br gDisplayMain;
extern UnkStruct_022c634c_br gDisplaySub;
extern UnkStruct_022c634c_f754 data_0208f754;

void GX_DisableBankForBG(void);
void GX_DisableBankForOBJ(void);
void GX_DisableBankForBGExtPltt(void);
void GX_DisableBankForOBJExtPltt(void);
void GX_DisableBankForTex(void);
void GX_DisableBankForTexPltt(void);
void GX_DisableBankForClearImage(void);
void GX_DisableBankForSubBG(void);
void GX_DisableBankForSubOBJ(void);
void GX_DisableBankForSubBGExtPltt(void);
void GX_DisableBankForSubOBJExtPltt(void);
void GX_DisableBankForARM7(void);
void GX_DisableBankForLCDC(void);
void Vram_SetBankForOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void ObjVram_Reset(s32 i);
void Vram_SetBankForBG(s32 a);
void Vram_SetBankForTex(s32 a);
void Vram_SetBankForTexPltt(s32 a);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void G3X_SetClearColor(u32 a, u32 b, u16 c, u32 d, BOOL e);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForSubBG(s32 a);
void GXS_SetGraphicsMode(u32 mode);
void GXx_SetMasterBrightness_(vu16 *reg, s32 b);

void ExtraEnding_InitDisplay(void) {
    *(vu16 *)0x4000304 = (*(vu16 *)0x4000304 & ~0x20e) | 0x20e;
    *(vu16 *)0x4000304 |= 0x8000;
    GX_DisableBankForBG();
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
    Vram_SetBankForOBJ(0x40, 0x100010, 0x40, 0, 0x100);
    ObjVram_Reset(0);
    Vram_SetBankForBG(0x10);
    Vram_SetBankForTex(0xb);
    Vram_SetBankForTexPltt(0x20);
    GX_SetGraphicsMode(1, 0, 1);
    *(vu32 *)0x4000000 &= ~0x38000000;
    *(vu32 *)0x4000000 &= ~0x7000000;
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 1;
    *(vu16 *)0x4000060 = *(vu16 *)0x4000060 & ~0x3002;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x10;
    *(vu16 *)0x4000060 = *(vu16 *)0x4000060 & 0xcffb;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 8;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x20;
    G3X_SetClearColor(0, 0, 0x7fff, 0, 0);
    *(vu32 *)0x4000540 = (data_0208f754.unk84 << 1) | data_0208f754.unk80;
    *(vu32 *)0x4000580 = 0xbfff0000;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x600;
    *(vu16 *)0x4000008 = (*(vu16 *)0x4000008 & ~3) | 1;
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & ~3) | 3;
    *(vu16 *)0x400000c = *(vu16 *)0x400000c & ~3;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & ~3) | 2;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1100;
    Vram_SetBankForSubOBJ(0x100, 0x100010, 0x40, 0, 0x100);
    ObjVram_Reset(1);
    Vram_SetBankForSubBG(4);
    GXS_SetGraphicsMode(0);
    *(vu16 *)0x4001008 = (*(vu16 *)0x4001008 & 0x43) | 0x2788;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 0x2698;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & 0x43) | 0x700;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & 0x43) | 0x900;
    *(vu16 *)0x4001008 = (*(vu16 *)0x4001008 & ~3) | 1;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & ~3) | 3;
    *(vu16 *)0x400100c = *(vu16 *)0x400100c & ~3;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & ~3) | 2;
    gDisplayMain.unk48 = 0x10;
    gDisplaySub.unk48 = 0x10;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, gDisplayMain.unk48);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, gDisplaySub.unk48);
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1c00;
}
