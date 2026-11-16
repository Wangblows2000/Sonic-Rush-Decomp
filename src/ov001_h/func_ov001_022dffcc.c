#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct UnkStruct_0208f754 {
    u8 pad0[0x80];
    s32 unk80;
    vs32 unk84;
} UnkStruct_0208f754;

typedef struct UnkStruct_022dffcc_sub {
    u8 pad0[0x68];
    u32 unk68;
    u8 pad6C[0x92 - 0x6c];
    u16 unk92;
    u16 unk94;
} UnkStruct_022dffcc_sub;

typedef struct UnkMtx22_022dffcc {
    s32 m[4];
} UnkMtx22_022dffcc;

extern UnkStruct_0208f754 data_0208f754;
extern UnkStruct_022dffcc_sub gDisplaySub;
extern u8 data_02087d2c[];

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
void Vram_SetBankForTex(s32 a);
void Vram_ResetTexAndPltt(void);
void Vram_SetBankForTexPltt(s32 a);
void VramPltt_Reset(void);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void G3X_SetClearColor(s32 a, s32 b, s32 c, s32 d, s32 e);
void ObjVram_Reset(s32 a);
void GX_SetBankForSubOBJ(s32 a);
void Vram_SetBankForSubBG(s32 a);
void GXS_SetGraphicsMode(s32 mode);
void MTX_Identity22_(UnkMtx22_022dffcc *m);
void G2x_SetBGyAffine_(void *p, UnkMtx22_022dffcc *m, s32 x, s32 y, s32 a, s32 b);

void func_ov001_022dffcc(void) {
    UnkMtx22_022dffcc mtx;

    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    REG16(0x4000304) |= 0x8000;
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
    Vram_SetBankForTex(3);
    Vram_ResetTexAndPltt();
    Vram_SetBankForTexPltt(0x10);
    VramPltt_Reset();
    GX_SetGraphicsMode(1, 5, 1);
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 1;
    REG16(0x4000060) &= ~0x3002;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x10;
    REG16(0x4000060) &= (u16)~0x3004;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x8;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x20;
    G3X_SetClearColor(0, 0, 0x7fff, 0, 0);
    {
        s32 zw = data_0208f754.unk84;
        s32 am = data_0208f754.unk80;
        REG32(0x4000540) = am | (zw << 1);
    }
    REG32(0x4000580) = 0xbfff0000;
    REG16(0x4000008) &= ~3;
    REG32(0x4000000) = (REG32(0x4000000) & ~0x1f00) | 0x100;
    ObjVram_Reset(1);
    gDisplaySub.unk94 = 2;
    gDisplaySub.unk92 = 0x80;
    gDisplaySub.unk68 = 0x6860000;
    GX_SetBankForSubOBJ(8);
    REG32(0x4001000) = (REG32(0x4001000) & ~0x60) | 0x20;
    Vram_SetBankForSubBG(4);
    GXS_SetGraphicsMode(5);
    REG16(0x400100c) = (REG16(0x400100c) & 0x43) | 0x4084;
    MTX_Identity22_(&mtx);
    G2x_SetBGyAffine_(data_02087d2c, &mtx, 0, 0, 0, 0);
    REG16(0x400100c) &= ~3;
    REG32(0x4001000) = (REG32(0x4001000) & ~0x1f00) | 0x1400;
}
