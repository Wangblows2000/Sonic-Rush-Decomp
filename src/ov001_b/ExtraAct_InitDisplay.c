#include <nitro/types.h>

typedef struct UnkStruct_022f95ec_bl {
    s16 x;
    s16 y;
} UnkStruct_022f95ec_bl;

typedef struct UnkStruct_022f95ec_br {
    UnkStruct_022f95ec_bl ofs[4];
    u8 pad10[0xc];
    u32 unk1C;
    u8 pad20[0x28];
    s16 unk48;
} UnkStruct_022f95ec_br;

typedef struct UnkStruct_022f95ec_f754 {
    u8 pad[0x80];
    u32 unk80;
    u32 unk84;
} UnkStruct_022f95ec_f754;

typedef struct UnkStruct_022f95ec_4960 {
    u8 pad[0x9c];
    u16 unk9C;
} UnkStruct_022f95ec_4960;

typedef struct UnkStruct_022f95ec_48bc {
    u8 pad[0x90];
    s16 v[9];
} UnkStruct_022f95ec_48bc;

extern UnkStruct_022f95ec_br gDisplayMain;
extern UnkStruct_022f95ec_br gDisplaySub;
extern UnkStruct_022f95ec_f754 data_0208f754;
extern UnkStruct_022f95ec_4960 data_02074960;
extern UnkStruct_022f95ec_48bc data_020748bc;

void GX_DispOff(void);
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
void GX_DispOn(void);
void G3X_SetClearColor(u32 a, u32 b, u16 c, u32 d, BOOL e);
void Vram_SetBankForBG(s32 a);
void Vram_SetBankForOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForTex(s32 a);
void Vram_SetBankForTexPltt(s32 a);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void Vram_SetBankForBGExtPltt(s32 a);
void Vram_SetBankForOBJExtPltt(s32 a);
void GXx_SetMasterBrightness_(vu16 *reg, s32 b);
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void GXS_SetGraphicsMode(u32 mode);
void Vram_SetBankForSubBGExtPltt(s32 a);
void ObjVram_Reset(s32 i);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void func_02022270(u32 index, u32 value);
void func_02022284(u32 idx, s32 x, s32 y, s32 z);

void ExtraAct_InitDisplay(void) {
    GX_DispOff();
    *(vu32 *)0x4001000 &= ~0x10000;
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
    *(vu16 *)0x5000000 = 0;
    *(vu16 *)0x5000400 = 0;
    gDisplayMain.unk48 = gDisplaySub.unk48 = 0x10;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, 0x10);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, 0x10);
    *(vu32 *)0x4000000 &= ~0x1f00;
    *(vu32 *)0x4001000 &= ~0x1f00;
    *(vu16 *)0x4000304 |= 0x8000;
    *(vu16 *)0x4000304 = (*(vu16 *)0x4000304 & ~0x20e) | 0x20e;
    GX_DispOn();
    *(vu32 *)0x4001000 |= 0x10000;
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 0;
    *(vu32 *)0x4000540 = data_0208f754.unk80 | (data_0208f754.unk84 << 1);
    *(vu16 *)0x4000060 = *(vu16 *)0x4000060 & ~0x3002;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x10;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 4;
    *(vu16 *)0x4000340 = 0;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 8;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x20;
    G3X_SetClearColor(0, 0, 0x7fff, 0, 0);
    *(vu32 *)0x4000580 = 0xbfff0000;
    Vram_SetBankForBG(4);
    Vram_SetBankForOBJ(0x10, 0x100010, 0x40, 0, 0x100);
    Vram_SetBankForOBJExtPltt(0);
    Vram_SetBankForTex(3);
    Vram_SetBankForTexPltt(0x20);
    GX_SetGraphicsMode(1, 1, 1);
    Vram_SetBankForBGExtPltt(0x40);
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 0x2184;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 8;
    *(vu16 *)0x4000008 = (*(vu16 *)0x4000008 & ~3) | 1;
    *(vu16 *)0x400000a = *(vu16 *)0x400000a & ~3;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & ~3) | 2;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & ~3) | 3;
    *(vu32 *)0x4000010 = 0;
    gDisplayMain.ofs[0].x = 0;
    gDisplayMain.ofs[0].y = 0;
    *(vu32 *)0x4000014 = 0;
    gDisplayMain.ofs[1].x = 0;
    gDisplayMain.ofs[1].y = 0;
    *(vu32 *)0x4000018 = 0;
    gDisplayMain.ofs[2].x = 0;
    gDisplayMain.ofs[2].y = 0;
    *(vu32 *)0x400001c = 0;
    gDisplayMain.ofs[3].x = 0;
    gDisplayMain.ofs[3].y = 0;
    *(vu32 *)0x4001010 = 0;
    gDisplaySub.ofs[0].x = 0;
    gDisplaySub.ofs[0].y = 0;
    *(vu32 *)0x4001014 = 0;
    gDisplaySub.ofs[1].x = 0;
    gDisplaySub.ofs[1].y = 0;
    *(vu32 *)0x4001018 = 0;
    gDisplaySub.ofs[2].x = 0;
    gDisplaySub.ofs[2].y = 0;
    *(vu32 *)0x400101c = 0;
    gDisplaySub.ofs[3].x = 0;
    gDisplaySub.ofs[3].y = 0;
    gDisplayMain.unk1C = 0;
    *(vu32 *)0x4000000 &= ~0xe000;
    gDisplaySub.unk1C = 0;
    *(vu32 *)0x4001000 &= ~0xe000;
    ObjVram_Reset(0);
    ObjVram_Reset(1);
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    func_02022270(0, 0x7fff);
    func_02022270(1, 0x4631);
    func_02022270(2, 0x2108);
    func_02022284(0, 0x4a0, -0x4a0, 0);
    func_02022284(1, 0, -0x617, -0xecd);
    func_02022284(2, 0xec7, 0x620, 0);
}
