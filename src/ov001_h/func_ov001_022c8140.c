#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct UnkBright_022c8140 {
    u8 pad0[0x48];
    s16 brightness;
} UnkBright_022c8140;

extern UnkBright_022c8140 gDisplayMain;
extern UnkBright_022c8140 gDisplaySub;

void func_02002ec8(void);
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
void ObjVram_Reset(s32 a);
void GXS_SetGraphicsMode(s32 mode);
void GX_DispOn(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

void func_ov001_022c8140(void) {
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
    Vram_SetBankForBG(3);
    Vram_SetBankForBGExtPltt(0x60);
    func_02002ab0(1, 3, 0);
    REG16(0x4000008) = (REG16(0x4000008) & 0x43) | 0x9c;
    REG16(0x400000a) = (REG16(0x400000a) & 0x43) | 0x184;
    REG16(0x400000c) = (REG16(0x400000c) & 0x43) | 0x428c;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x4414;
    Vram_SetBankForSubOBJ(0x100, 0x10, 0x40, 0, 0x200);
    ObjVram_Reset(1);
    Vram_SetBankForSubBG(4);
    GXS_SetGraphicsMode(0);
    REG16(0x400100a) = (REG16(0x400100a) & 0x43) | 4;
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    REG16(0x4000008) = (REG16(0x4000008) & ~3);
    REG16(0x400000e) = (REG16(0x400000e) & ~3) | 1;
    REG16(0x400000c) = (REG16(0x400000c) & ~3) | 2;
    REG16(0x400000a) = (REG16(0x400000a) & ~3) | 3;
    REG16(0x400100a) = (REG16(0x400100a) & ~3);
    REG16(0x4001008) = (REG16(0x4001008) & ~3) | 1;
    REG16(0x400100c) = (REG16(0x400100c) & ~3) | 2;
    REG16(0x400100e) = (REG16(0x400100e) & ~3) | 3;
    REG16(0x4000304) |= 0x8000;
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    GX_DispOn();
    REG32(0x4001000) |= 0x10000;
    gDisplayMain.brightness = 16;
    gDisplaySub.brightness = 16;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, 16);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, 16);
    {
        vu32 z = 0;
        MIi_CpuClear32(z, (void *)0x6000000, 0x4000);
    }
}
