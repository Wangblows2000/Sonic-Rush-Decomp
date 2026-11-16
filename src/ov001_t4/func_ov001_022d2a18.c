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
} UnkStruct_022d2a18_Ofs;

typedef struct {
    UnkStruct_022d2a18_Ofs ofs[4];
} UnkStruct_022d2a18_Bg;

typedef struct {
    u8 pad[8];
    s16 brightness;
} UnkStruct_022d2a18_Br;

typedef struct {
    u32 unk[4];
} UnkStruct_022d2a18_Mtx;

extern UnkStruct_0208f7d4 data_0208f7d4;
extern UnkStruct_022d2a18_Bg gDisplayMain;
extern UnkStruct_022d2a18_Bg gDisplaySub;
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];
extern UnkStruct_022d2a18_Br data_0208926c;
extern UnkStruct_022d2a18_Br data_02087d44;

void func_02002f60(void);
void func_02002f4c(void);
void func_02002f28(void);
void func_02002f04(void);
void func_02002ef0(void);
void func_02002edc(void);
void func_02002ec8(void);
void func_02002e8c(void);
void func_02002e78(void);
void func_02002e50(void);
void func_02002e28(void);
void func_02002eb4(void);
void func_02002ea0(void);
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
void func_02002ab0(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 mode);
void ObjVram_Reset(s32 a);
void Vram_ResetTexAndPltt(void);
void VramPltt_Reset(void);
void func_02004040(s32 a, s32 b, s32 c, s32 d, s32 e);
void GX_DispOn(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02000cb4(UnkStruct_022d2a18_Mtx *m);
void func_02003d94(void *a, UnkStruct_022d2a18_Mtx *m, s32 c, s32 d, s32 e, s32 f);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

#pragma thumb on

void func_ov001_022d2a18(s32 *p) {
    vu32 z0;
    vu32 z1;
    s32 i;

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
    Vram_SetBankForBG(2);
    Vram_SetBankForBGExtPltt(0x10);
    Vram_SetBankForOBJ(1, 0x200010, 0x40, 0, 0x400);
    ObjVram_Reset(0);
    Vram_SetBankForOBJExtPltt(0x40);
    func_02002ab0(1, 0, 0);
    REG16(0x4000008) = (REG16(0x4000008) & 0x43) | 0x14;
    REG16(0x400000a) = (REG16(0x400000a) & 0x43) | 0x10c;
    REG16(0x400000c) = (REG16(0x400000c) & 0x43) | 0x204;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x380;
    Vram_SetBankForSubBG(4);
    Vram_SetBankForSubBGExtPltt(0x80);
    Vram_SetBankForSubOBJ(8, 0x200010, 0x40, 0, 0x400);
    Vram_SetBankForSubOBJExtPltt(0x100);
    ObjVram_Reset(1);
    GXS_SetGraphicsMode(0);
    REG16(0x4001008) = (REG16(0x4001008) & 0x43) | 0x14;
    REG16(0x400100a) = (REG16(0x400100a) & 0x43) | 0x10c;
    REG16(0x400100c) = (REG16(0x400100c) & 0x43) | 0x204;
    REG16(0x400100e) = (REG16(0x400100e) & 0x43) | 0x380;
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    REG16(0x4000008) = (REG16(0x4000008) & ~3) | 1;
    REG16(0x400000a) = (REG16(0x400000a) & ~3);
    REG16(0x400000c) = (REG16(0x400000c) & ~3) | 2;
    REG16(0x400000e) = (REG16(0x400000e) & ~3) | 3;
    REG16(0x4001008) = (REG16(0x4001008) & ~3) | 1;
    REG16(0x400100a) = (REG16(0x400100a) & ~3);
    REG16(0x400100c) = (REG16(0x400100c) & ~3) | 2;
    REG16(0x400100e) = (REG16(0x400100e) & ~3) | 3;
    for (i = 0; i < 4; i++) {
        gDisplayMain.ofs[i].x = 0;
        gDisplayMain.ofs[i].y = 0;
        gDisplaySub.ofs[i].x = 0;
        gDisplaySub.ofs[i].y = 0;
    }
    REG16(0x4000304) |= 0x8000;
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    GX_DispOn();
    REG32(0x4001000) |= 0x10000;
    if (*p == 2) {
        data_0208926c.brightness = -16;
        data_02087d44.brightness = -16;
        GXx_SetMasterBrightness_((vu16 *)0x400006c, -16);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, -16);
    } else {
        data_0208926c.brightness = 0x10;
        data_02087d44.brightness = 0x10;
        GXx_SetMasterBrightness_((vu16 *)0x400006c, 0x10);
        GXx_SetMasterBrightness_((vu16 *)0x400106c, 0x10);
    }
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000000, 0x20000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200000, 0x20000);
}
#pragma thumb reset
