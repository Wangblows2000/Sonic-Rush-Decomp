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
} UnkStruct_022cf93c_Ofs;

typedef struct {
    UnkStruct_022cf93c_Ofs ofs[4];
} UnkStruct_022cf93c_Bg;

typedef struct {
    u8 pad[8];
    s16 brightness;
} UnkStruct_022cf93c_Br;

typedef struct {
    u32 unk[4];
} UnkStruct_022cf93c_Mtx;

extern UnkStruct_0208f7d4 data_0208f7d4;
extern UnkStruct_022cf93c_Bg gDisplayMain;
extern UnkStruct_022cf93c_Bg gDisplaySub;
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];
extern UnkStruct_022cf93c_Br data_0208926c;
extern UnkStruct_022cf93c_Br data_02087d44;

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
void Vram_SetBankConfig(s32 a);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MTX_Identity22_(UnkStruct_022cf93c_Mtx *m);
void G2x_SetBGyAffine_(void *a, UnkStruct_022cf93c_Mtx *m, s32 c, s32 d, s32 e, s32 f);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

#pragma thumb on

void SegaLogo_InitDisplay(void) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;

    Vram_SetBankConfig(0);
    Vram_SetBankForBGExtPltt(0);
    Vram_SetBankForSubBGExtPltt(0);
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    REG16(0x4000304) |= 0x8000;
    GX_SetGraphicsMode(1, 0, 0);
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 4;
    GXS_SetGraphicsMode(0);
    REG16(0x400100e) = (REG16(0x400100e) & 0x43) | 4;
    if (data_0208926c.brightness == 0x10) {
        data_0208926c.brightness = 0x10;
        data_02087d44.brightness = 0x10;
    } else {
        data_0208926c.brightness = -16;
        data_02087d44.brightness = -16;
    }
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
    REG32(0x4000000) = (REG32(0x4000000) & ~0x1f00) | 0x800;
    REG32(0x4001000) = (REG32(0x4001000) & ~0x1f00) | 0x800;
}
#pragma thumb reset
