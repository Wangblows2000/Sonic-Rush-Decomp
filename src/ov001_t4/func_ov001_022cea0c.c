#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct {
    vs32 unk0;
    vs32 unk4;
} UnkStruct_0208f7d4;

typedef struct {
    vu16 x;
    vu16 y;
} UnkStruct_022cea0c_Ofs;

typedef struct {
    UnkStruct_022cea0c_Ofs ofs[4];
} UnkStruct_022cea0c_Bg;

typedef struct {
    u8 pad[8];
    s16 brightness;
} UnkStruct_022cea0c_Br;

typedef struct {
    u32 unk[4];
} UnkStruct_022cea0c_Mtx;

extern UnkStruct_0208f7d4 data_0208f7d4;
extern UnkStruct_022cea0c_Bg gDisplayMain;
extern UnkStruct_022cea0c_Bg gDisplaySub;
extern u8 data_02089254[];
extern u8 data_02089264[];
extern u8 data_02087d2c[];
extern u8 data_02087d3c[];
extern UnkStruct_022cea0c_Br data_0208926c;
extern UnkStruct_022cea0c_Br data_02087d44;

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
void func_02049070(void);
void Vram_SetBankConfig(s32 a);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_02000cb4(UnkStruct_022cea0c_Mtx *m);
void func_02003d94(void *a, UnkStruct_022cea0c_Mtx *m, s32 c, s32 d, s32 e, s32 f);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);

#pragma thumb on

void func_ov001_022cea0c(void) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;

    REG16(0x5000000) = 0;
    REG16(0x5000400) = 0;
    REG32(0x4000000) = REG32(0x4000000) & ~0x1f00;
    REG32(0x4001000) = REG32(0x4001000) & ~0x1f00;
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    REG16(0x4000304) |= 0x8000;
    func_02049070();
    Vram_SetBankConfig(0);
    Vram_ResetTexAndPltt();
    VramPltt_Reset();
    func_02002ab0(1, 0, 0);
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    REG16(0x4000008) = (REG16(0x4000008) & 0x43) | 0x4088;
    REG16(0x400000a) = (REG16(0x400000a) & 0x43) | 0x428c;
    REG16(0x400000c) = (REG16(0x400000c) & 0x43) | 0x490;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x694;
    GXS_SetGraphicsMode(0);
    REG16(0x4001008) = (REG16(0x4001008) & 0x43) | 0x4088;
    REG16(0x400100a) = (REG16(0x400100a) & 0x43) | 0x428c;
    REG16(0x400100c) = (REG16(0x400100c) & 0x43) | 0x490;
    REG16(0x400100e) = (REG16(0x400100e) & 0x43) | 0x694;
    REG16(0x4000008) = (REG16(0x4000008) & ~3);
    REG16(0x400000a) = (REG16(0x400000a) & ~3) | 1;
    REG16(0x400000c) = (REG16(0x400000c) & ~3) | 2;
    REG16(0x400000e) = (REG16(0x400000e) & ~3) | 3;
    REG16(0x4001008) = (REG16(0x4001008) & ~3) | 1;
    REG16(0x400100a) = (REG16(0x400100a) & ~3) | 2;
    REG16(0x400100c) = (REG16(0x400100c) & ~3);
    REG16(0x400100e) = (REG16(0x400100e) & ~3) | 3;
    gDisplayMain.ofs[0].x = gDisplayMain.ofs[0].y = gDisplayMain.ofs[1].x = gDisplayMain.ofs[1].y = gDisplayMain.ofs[2].x = gDisplayMain.ofs[2].y = gDisplayMain.ofs[3].x = gDisplayMain.ofs[3].y = gDisplaySub.ofs[0].x = gDisplaySub.ofs[0].y = gDisplaySub.ofs[1].x = gDisplaySub.ofs[1].y = gDisplaySub.ofs[2].x = gDisplaySub.ofs[2].y = gDisplaySub.ofs[3].x = gDisplaySub.ofs[3].y = 0;
    REG32(0x4000010) = 0;
    REG32(0x4000014) = 0;
    REG32(0x4000018) = 0;
    REG32(0x400001c) = 0;
    REG32(0x4001010) = 0;
    REG32(0x4001014) = 0;
    REG32(0x4001018) = 0;
    REG32(0x400101c) = 0;
    data_0208926c.brightness = data_02087d44.brightness = -16;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, -16);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, -16);
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
}
#pragma thumb reset
