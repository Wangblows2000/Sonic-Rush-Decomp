#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct {
    vs32 unk0;
    vs32 unk4;
} UnkStruct_0208f7d4;

typedef struct {
    s16 x;
    s16 y;
} UnkStruct_022d5114_Ofs;

typedef struct {
    UnkStruct_022d5114_Ofs ofs[4];
} UnkStruct_022d5114_Bg;

typedef struct {
    u8 pad[8];
    s16 brightness;
} UnkStruct_022d5114_Br;

typedef struct {
    u32 unk[4];
} UnkStruct_022d5114_Mtx;

extern UnkStruct_0208f7d4 data_0208f7d4;
extern UnkStruct_022d5114_Bg gDisplayMain;
extern UnkStruct_022d5114_Bg gDisplaySub;
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];
extern UnkStruct_022d5114_Br data_0208926c;
extern UnkStruct_022d5114_Br data_02087d44;

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
void Vram_SetBankForBG(s32 a);
void Vram_SetBankForOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForBGExtPltt(s32 a);
void Vram_SetBankForOBJExtPltt(s32 a);
void Vram_SetBankForTex(s32 a);
void Vram_SetBankForTexPltt(s32 a);
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForSubBGExtPltt(s32 a);
void Vram_SetBankForSubOBJExtPltt(s32 a);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 mode);
void ObjVram_Reset(s32 a);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void G3X_SetClearColor(s32 a, s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MTX_Identity22_(UnkStruct_022d5114_Mtx *m);
void G2x_SetBGyAffine_(void *a, UnkStruct_022d5114_Mtx *m, s32 c, s32 d, s32 e, s32 f);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

#pragma thumb on

void TitleDemo_InitDisplay(void) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;
    UnkStruct_022d5114_Mtx mtx;
    s32 i;

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
    GX_DisableBankForClearImage();
    GX_DisableBankForARM7();
    GX_DisableBankForLCDC();
    Vram_SetBankForOBJ(0x10, 0x100010, 0x40, 0, 0x400);
    Vram_SetBankForOBJExtPltt(0);
    ObjVram_Reset(0);
    Vram_SetBankForBG(2);
    Vram_SetBankForBGExtPltt(0x40);
    Vram_SetBankForSubOBJ(8, 0x200010, 0x40, 0, 0x400);
    Vram_SetBankForSubOBJExtPltt(0x100);
    ObjVram_Reset(1);
    Vram_SetBankForSubBG(4);
    Vram_SetBankForSubBGExtPltt(0x80);
    Vram_SetBankForTex(1);
    Vram_SetBankForTexPltt(0x20);
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    REG16(0x4000304) &= ~0x8000;
    GX_SetGraphicsMode(1, 0, 1);
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    data_0208f7d4.unk0 = 0;
    data_0208f7d4.unk4 = 0;
    REG16(0x4000060) &= ~0x3002;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x10;
    REG16(0x4000060) &= (u16)~0x3004;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x8;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x20;
    G3X_SetClearColor(0x7fff, 0, 0x7fff, 0, 0);
    {
        s32 zw = data_0208f7d4.unk4;
        s32 am = data_0208f7d4.unk0;
        REG32(0x4000540) = am | (zw << 1);
    }
    REG32(0x4000580) = 0xbfff0000;
    REG16(0x400000a) = (REG16(0x400000a) & 0x43) | 0x404;
    REG16(0x400000c) = (REG16(0x400000c) & 0x43) | 0x8090;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x8298;
    GXS_SetGraphicsMode(3);
    REG16(0x400100a) = (REG16(0x400100a) & 0x43) | 0x408;
    REG16(0x400100e) = (REG16(0x400100e) & 0x43) | 0x4004;
    REG16(0x4000008) = (REG16(0x4000008) & ~3);
    REG16(0x400000c) = (REG16(0x400000c) & ~3) | 1;
    REG16(0x400000e) = (REG16(0x400000e) & ~3) | 2;
    REG16(0x400000a) = (REG16(0x400000a) & ~3) | 3;
    REG16(0x400100e) = (REG16(0x400100e) & ~3);
    REG16(0x4001008) = (REG16(0x4001008) & ~3) | 1;
    REG16(0x400100a) = (REG16(0x400100a) & ~3) | 2;
    REG16(0x400100c) = (REG16(0x400100c) & ~3) | 3;
    MTX_Identity22_(&mtx);
    gDisplayMain.ofs[0].x = 0;
    gDisplayMain.ofs[0].y = 0;
    gDisplayMain.ofs[1].x = 0;
    gDisplayMain.ofs[1].y = 0;
    gDisplayMain.ofs[2].x = 0;
    gDisplayMain.ofs[2].y = 0;
    gDisplayMain.ofs[3].x = 0;
    gDisplayMain.ofs[3].y = 0;
    gDisplaySub.ofs[0].x = 0;
    gDisplaySub.ofs[0].y = 0;
    gDisplaySub.ofs[1].x = 0;
    gDisplaySub.ofs[1].y = 0;
    gDisplaySub.ofs[2].x = 0;
    gDisplaySub.ofs[2].y = 0;
    gDisplaySub.ofs[3].x = 0;
    gDisplaySub.ofs[3].y = 0;
    G2x_SetBGyAffine_(data_02089254, &mtx, 0, 0, 0, 0);
    G2x_SetBGyAffine_(data_02089264, &mtx, 0, 0, 0, 0);
    G2x_SetBGyAffine_(data_02087d2c, &mtx, 0, 0, 0, 0);
    G2x_SetBGyAffine_(data_02087d3c, &mtx, 0, 0, 0, 0);
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000000, 0x80000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200000, 0x20000);
    z2 = 0;
    MIi_CpuClear32(z2, (void *)0x5000000, 0x200);
    z3 = 0;
    MIi_CpuClear32(z3, (void *)0x5000400, 0x200);
    z4 = 0;
    MIi_CpuClear32(z4, (void *)0x5000200, 0x200);
    z5 = 0;
    MIi_CpuClear32(z5, (void *)0x5000600, 0x200);
    REG16(0x5000000) = 0x7fff;
    REG16(0x5000400) = 0x7fff;
    data_0208926c.brightness = 0x10;
    data_02087d44.brightness = 0x10;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208926c.brightness);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d44.brightness);
    REG32(0x4000000) = (REG32(0x4000000) & ~0x1f00) | 0x1000;
    REG32(0x4001000) = (REG32(0x4001000) & ~0x1f00) | 0x1000;
    {
        vu16 *p;
        for (i = 0, p = (vu16 *)0x5000000; i < 16; i++) {
            *p++ = 0x7fff;
        }
    }
    {
        u32 *p;
        for (i = 0, p = (u32 *)0x6004000; i < 8; i++) {
            *p++ = ~0;
        }
    }
    {
        vu16 *p;
        for (i = 0, p = (vu16 *)0x5000400; i < 16; i++) {
            *p++ = 0x7fff;
        }
    }
    {
        u32 *p;
        for (i = 0, p = (u32 *)0x6208000; i < 8; i++) {
            *p++ = ~0;
        }
    }
}
#pragma thumb reset
