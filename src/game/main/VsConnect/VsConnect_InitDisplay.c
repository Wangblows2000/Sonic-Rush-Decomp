#include <nitro/types.h>

typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02062d44;
typedef struct { u16 v[8]; } UnkMosaic_02062d44;
extern UnkBlend_02062d44 data_0208926c;
extern UnkBlend_02062d44 data_02087d44;
extern UnkMosaic_02062d44 gDisplayMain;
extern UnkMosaic_02062d44 gDisplaySub;

void Vram_SetBankForBG(s32 a);
void Vram_SetBankForOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForBGExtPltt(s32 a);
void Vram_SetBankForOBJExtPltt(s32 a);
void Vram_SetBankForTex(s32 a);
void Vram_SetBankForTexPltt(s32 a);
void GX_DisableBankForClearImage(void);
void Vram_SetBankForSubBG(s32 a);
void Vram_SetBankForSubOBJ(s32 a, s32 b, s32 c, s32 d, s32 e);
void Vram_SetBankForSubBGExtPltt(s32 a);
void Vram_SetBankForSubOBJExtPltt(s32 a);
void GX_DisableBankForARM7(void);
void GX_DisableBankForLCDC(void);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 mode);
void ObjVram_Reset(s32 a);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
static inline void Clear32_02062d44(void *dst, u32 size) {
    vu32 z = 0;
    MIi_CpuClear32(z, dst, size);
}

void VsConnect_InitDisplay(void) {
    s16 b;

    *(vu32 *)0x04000000 = *(vu32 *)0x04000000 & ~0x1f00;
    *(vu32 *)0x04001000 = *(vu32 *)0x04001000 & ~0x1f00;
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
    *(vu16 *)0x04000304 = (*(vu16 *)0x04000304 & ~0x20e) | 0x20e;
    *(vu16 *)0x04000304 |= 0x8000;
    GX_SetGraphicsMode(1, 0, 0);
    *(vu32 *)0x04000000 &= ~0x38000000;
    *(vu32 *)0x04000000 &= ~0x07000000;
    GXS_SetGraphicsMode(0);
    Vram_SetBankForOBJ(0x10, 0x100010, 0x40, 0, 0x400);
    Vram_SetBankForOBJExtPltt(0);
    ObjVram_Reset(0);
    Vram_SetBankForBG(2);
    Vram_SetBankForBGExtPltt(0x40);
    Vram_SetBankForSubOBJ(8, 0x200010, 0x40, 0, 0x400);
    Vram_SetBankForSubOBJExtPltt(0);
    ObjVram_Reset(1);
    Vram_SetBankForSubBG(4);
    Vram_SetBankForSubBGExtPltt(0x80);
    Vram_SetBankForTex(1);
    Vram_SetBankForTexPltt(0x20);
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & 0x43) | 0x2084;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & 0x43) | 0x2188;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0x28c;
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & 0x43) | 0x390;
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & 0x43) | 0x4084;
    *(vu16 *)0x0400100a = (*(vu16 *)0x0400100a & 0x43) | 0x4188;
    *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & 0x43) | 0x28c;
    *(vu16 *)0x0400100e = (*(vu16 *)0x0400100e & 0x43) | 0x390;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 0;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 1;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 2;
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & ~3) | 3;
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & ~3) | 0;
    *(vu16 *)0x0400100a = (*(vu16 *)0x0400100a & ~3) | 1;
    *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & ~3) | 2;
    *(vu16 *)0x0400100e = (*(vu16 *)0x0400100e & ~3) | 3;
    gDisplayMain.v[0] = 0;
    gDisplayMain.v[1] = 0;
    gDisplayMain.v[2] = 0;
    gDisplayMain.v[3] = 0;
    gDisplayMain.v[4] = 0;
    gDisplayMain.v[5] = 0;
    gDisplayMain.v[6] = 0;
    gDisplayMain.v[7] = 0;
    gDisplaySub.v[0] = 0;
    gDisplaySub.v[1] = 0;
    gDisplaySub.v[2] = 0;
    gDisplaySub.v[3] = 0;
    gDisplaySub.v[4] = 0;
    gDisplaySub.v[5] = 0;
    gDisplaySub.v[6] = 0;
    gDisplaySub.v[7] = 0;
    b = data_0208926c.unk8;
    if (b != 0x10 && b != -0x10) {
        data_0208926c.unk8 = -0x10;
        data_02087d44.unk8 = -0x10;
    }
    Clear32_02062d44((void *)0x06000000, 0x80000);
    Clear32_02062d44((void *)0x06200000, 0x20000);
    Clear32_02062d44((void *)0x05000000, 0x200);
    Clear32_02062d44((void *)0x05000400, 0x200);
    Clear32_02062d44((void *)0x05000200, 0x200);
    Clear32_02062d44((void *)0x05000600, 0x200);
}
#pragma thumb reset
